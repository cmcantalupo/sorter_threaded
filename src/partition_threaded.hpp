#ifndef partition_threaded_hpp
#define partition_threaded_hpp

#include <omp.h>
#include "partition.hpp"
#include "splinter.hpp"

template <class type>
class PartitionThreaded {
  public:
    PartitionThreaded(const std::set<type>& pivots, int maxThreads=-1);
    void part(typename std::vector<type>::iterator begin, 
              typename std::vector<type>::iterator end);
  private:
    std::set<type> pivots_;
    int maxThreads_;
};

PartitionThreaded::PartitionThreaded(const std::set<type>& pivots, int maxThreads=-1) :
  maxThreads_(maxThreads),
  pivots_(pivots) {}

void part(typename std::vector<type>::iterator begin,
          typename std::vector<type>::iterator end) {
#ifndef _OPENMP
  std::sort(begin, end);
#else
  // Get the number of threads and reset it if the attribute
  // maxThreads_ is smaller
  int numThreads = omp_get_max_threads();

  if (maxThreads_ != -1 && maxThreads_ < numThreads) {
    numThreads = maxThreads_;
  }

  // If there is just one thread use Partition()
  if (numThreads == 1) {
    SorterThreaded::Partition serialPart(pivots_);
    serialPart.fill(begin, end);
    std::vector<size_t> sizes;
    serialPart.
    for (int i = 0; i <= pivots_.size(); ++i) {
      
    }
    std::sort(begin,end);
    return;
  }

}
