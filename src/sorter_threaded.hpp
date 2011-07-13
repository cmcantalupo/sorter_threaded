// SorterThreaded class.  Has a sort() member function that will use
// OpenMP threading to sort a vector.  The taskFactor determines the
// number of tasks that the problem will be broken up into, where the
// number of tasks is the number of threads times the task factor.
// The number of threads used can be lowered from the
// omp_get_max_threads by setting maxThreads to a value other than -1.
// The scaling of the partitioning step is O(numEl*log(numTasks)) in
// time and the the sort algorithm is then called on each task.  The
// built in std::sort() is used if STL_SORT_THREAD_SAFE is defined,
// otherwise a thread safe quick_sort is used.
//
// C.M. Cantalupo 2011
// cmcantalupo@gmail.com

#ifndef st_sorter_threaded_hpp
#define st_sorter_threaded_hpp

#include <string>
#include <vector>
#include <algorithm>
#ifdef _OPENMP
#include <omp.h>
#include <iostream>
#endif
#include "partition.hpp"
#include "splinter.hpp"
#ifndef STL_SORT_THREAD_SAFE
#include "quick_sort.hpp"
#endif

template <class type>
class SorterThreaded {
  public:
    SorterThreaded(int taskFactor=8, int maxThreads=-1);
    void sort(typename std::vector<type>::iterator begin, 
              typename std::vector<type>::iterator end);
    void setTaskFactor(int taskFactor);
    void setMaxThreads(int maxThreads);
  private:
    int taskFactor_;
    // If numThreads_ == -1 then omp_get_max_threads will be used
    int maxThreads_;
};

  // We need to break the input vector into nearly equal size chunks
  // for partitioning.  One chunk for each thread. 
  //
  // We can build up a Partition from an array of non-repeating 
  // values (pivots) from the input vector.  
  //
  // Each thread makes its own Partition.  These are OpenMP private.
  // Each partition has a stack for each thread and each task.  This
  // implies that the total number of stacks over all threads is
  // numThreads^3 * taskFactor_.
 

template <class type>
void SorterThreaded<type>::sort(typename std::vector<type>::iterator begin, 
                          typename std::vector<type>::iterator end) {
  // To achieve parallelism here we will choose a set of pivots from 
  // the list to be sorted.  Here we will just choose the first elements
  // in the vector.  The number of pivots will determine the number of 
  // tasks to be done: one more task than the number of pivots. 
  //
  // For simplicity here the number of tasks is chosen to be a factor
  // of the number of threads.  This is determined by the class
  // attribute taskFactor_.
  //
  // We need to break down the main scaling dimension of the problem,
  // the length of the input vector.  The first thing that we want to
  // do with the input is to partition it into intervals bounded by
  // the pivots.  This is done with the std::map.upper_bound().  In
  // the end we want taskFactor_ times the number of threads jobs and
  // we need one fewer pivot than that to do so.  Map the intervals to
  // a stack with a std::map.
 
  // If there is no OpenMP just use std::sort()
#ifndef _OPENMP
  std::sort(begin, end);
#else
  // Get the number of threads and reset it if the attribute
  // maxThreads_ is smaller
  int numThreads = omp_get_max_threads();

  if (maxThreads_ != -1 && maxThreads_ < numThreads) {
    numThreads = maxThreads_;
  }

  // If there is just one thread use std::sort()
  if (numThreads == 1) {
    std::sort(begin,end);
    return;
  }

  int numTasks = numThreads * taskFactor_;

  std::set<type> pivots;
  // Create a set of pivots by going through the vector    
  for (typename std::vector<type>::iterator it = begin;
       it != end && pivots.size() < numThreads*taskFactor_ - 1;
       ++it) {
      pivots.insert(*it);
  }
  // Check that there were as many unique values as we need otherwise
  // use std::sort()
  if (pivots.size() < numThreads * taskFactor_ - 1) {
    std::sort(begin, end);
    return;
  }
  
  // Break the input vector into evenly sized chunks.  
  SorterThreadedHelper::Splinter<type> splinter(begin, end, numTasks);
  std::vector<typename std::vector<type>::iterator> chunks;
  splinter.even(numThreads, chunks);

  // taskOffsets is a shared variable, so declare it outside of the
  // omp parallel region
  std::vector<typename std::vector<type>::iterator> taskOffsets(numTasks);

  //This parallel region is for the partition step.  
#pragma omp parallel default (shared) num_threads (numThreads)
{
  int threadID = omp_get_thread_num();
  SorterThreadedHelper::Partition<type> partition(pivots);
  // Fill each thread's partition with a chunk of the vector.  
  partition.fill(chunks[threadID], chunks[threadID+1]);

  std::vector<size_t> mySizes;
  partition.taskSizes(mySizes);

  // Register each thread's partition sizes with Splinter
  for (int i = 0; i < numThreads; ++i) {
    if (i == threadID) {
      splinter.addSizes(mySizes);
    }
#pragma omp barrier
  }

  // Get the position to dump each thread's tasks back into the
  // vector.
  std::vector<typename std::vector<type>::iterator> offsets;
  for (int i = 0; i < numThreads; ++i) {
    if (i == threadID) {
      splinter.getOffsets(mySizes, offsets);
    }
#pragma omp barrier
  }
  
  // Refill the input vector with the partitioned values.  
  for (int i = 0; i < numTasks; ++i) {
    partition.popTask(offsets[i]);
  }

  // The last thread's offsets define the beginning of the partition so
  // copy them into the shared vector taskOffsets().
  if (threadID == numThreads - 1) {
    std::copy(offsets.begin(), offsets.end(), taskOffsets.begin());
  }
}

  // This parallel region is for sorting the partitioned intervals.  
#pragma omp parallel for schedule (dynamic) num_threads(numThreads) default(shared)
  for (int i = 0; i < numTasks; ++i) {
    if (i != numTasks - 1) {
#ifdef STL_SORT_THREAD_SAFE
      std::sort(taskOffsets[i], taskOffsets[i+1]);
#else
      SorterThreadedHelper::quick_sort<type>(taskOffsets[i], taskOffsets[i+1]);
#endif
    }
    else {
#ifdef STL_SORT_THREAD_SAFE
      std::sort(taskOffsets[numTasks - 1], end);
#else
      SorterThreadedHelper::quick_sort<type>(taskOffsets[numTasks - 1], end);
#endif
    }
  }
#endif //end of #ifdef _OPENMP
}

template <class type>
SorterThreaded<type>::SorterThreaded(int taskFactor, int maxThreads) :
  taskFactor_(taskFactor), 
  maxThreads_(maxThreads) {}

template <class type>
void SorterThreaded<type>::setMaxThreads(int maxThreads) {
  maxThreads_ = maxThreads;
}

template <class type>
void SorterThreaded<type>::setTaskFactor(int taskFactor) {
  taskFactor_ = taskFactor;
}

#endif
