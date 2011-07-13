// Unit test for quick_sort function.  
//
// C.M. Cantalupo 2011

#include "quick_sort.hpp"
#include <vector>
#include <algorithm>
#include <assert.h>

using namespace SorterThreadedHelper;

int main(int argc, char **argv) {
  size_t testSize = 10000;
  std::vector<double> orderedVector(testSize);
  
  for (size_t i = 0; i < testSize; i++) {
    orderedVector[i] = static_cast<double>(i);
  }
  std::vector<double> testVector(orderedVector);

  random_shuffle(testVector.begin(), testVector.end());
  std::vector<double>::iterator pivIt;
  int piv = 100;
  pivIt = part<double>(testVector.begin(), testVector.end(), piv);

  for (std::vector<double>::iterator it = testVector.begin();
       it != pivIt; it++) {
    assert(*it < piv);
  }

  for (std::vector<double>::iterator it = pivIt; 
       it != testVector.end(); it++) {
    assert(*it >= piv);
  }

  quick_sort<double>(testVector.begin(), testVector.end());

  assert(testVector == orderedVector);
    
}
