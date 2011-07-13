// Unit test for the Partition class.  
// 
// C.M. Cantalupo 2011
// cmcantalupo@gmail.com

#include <iostream>
#include <algorithm>
#include <assert.h>
#include "partition.hpp"

using namespace SorterThreadedHelper;

int main(int argc, char **argv) {
  int testSize = 100000;
  std::vector<double> testVec(testSize);
  for (int i = 0; i < testSize; ++i) {
    testVec[i] = i;
  }
  random_shuffle(testVec.begin(), testVec.end());
  int numPivots = 4;
  std::set<double> pivots;
  for (int i = 0; i < numPivots; i++) {
    pivots.insert(testVec[i]);
  }
  random_shuffle(testVec.begin(), testVec.end());

  Partition<double> part(pivots);
  part.fill(testVec.begin(), testVec.end());
  int numTasks = part.numTasks();
  assert(numTasks == numPivots+1);

  std::vector<size_t> taskSizes(numTasks);
  part.taskSizes(taskSizes);
  int sumTaskSizes = 0;
  for (int i = 0; i < numTasks; ++i) {
    sumTaskSizes += taskSizes[i];
  }
  assert(sumTaskSizes == testSize);

  std::vector<double> task(testSize);
  std::set<double>::iterator pivIt = pivots.begin();
  for (int i = 0; i < numTasks; ++i) {
    assert(i == part.curTask());
    part.popTask(task.begin());
    for (int j = 0; j < taskSizes[i]; j++ ) {
      if (i != numPivots)
        assert(task[j] < *pivIt);
      else 
        assert(task[j] >= *pivIt);
    }
    if (i != numPivots-1) {
      ++pivIt;
    }
  }
}
