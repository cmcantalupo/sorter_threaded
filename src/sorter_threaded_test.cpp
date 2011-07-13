// Unit test for SorterThreaded class.  Should be run with the
// environment variable OMP_NUM_THREADS set to a value greater than
// one.  
//
// C.M. Cantalupo 2011
// cmcantalupo@gmail.com

#include "sorter_threaded.hpp"
#include <algorithm>
#include <assert.h>


int main(int argc, char **argv) {
  size_t testSize = 1000000;
  std::vector<double> orderedVector(testSize);
  SorterThreaded<double> st;
  
  for (size_t i = 0; i < testSize; i++) {
    orderedVector[i] = static_cast<double>(i);
  }
  std::vector<double> testVector(orderedVector);

  random_shuffle(testVector.begin(), testVector.end());

  st.sort(testVector.begin(), testVector.end());

  assert(testVector == orderedVector);

}
