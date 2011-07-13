// Unit test for the SorterThreadedHelper::Splinter class.
//
// C.M. Cantalupo 2011
// cmcantalupo@gmail.com

#include "splinter.hpp"
#include "assert.h"

using namespace SorterThreadedHelper;

int main(int argc, char **argv) {
  size_t numChunk= 4;
  size_t testSize = 15;
  std::vector<double> testVec(testSize);
  for (int i = 0; i < testSize; ++i) {
    testVec[i] = static_cast<double>(i);
  }
  
  Splinter<double> sp(testVec.begin(), testVec.end(), 3);
  std::vector<std::vector<double>::iterator> chunks;
  sp.even(numChunk, chunks);
  assert(chunks.size() == numChunk + 1);
  
  for (int i = 0; i < numChunk; ++i) {
    if (i < 3) {
      assert(distance(chunks[i], chunks[i+1]) == 4);
    }
    else {
      assert(distance(chunks[i], chunks[i+1]) == 3);
    }
  }

  std::vector<size_t> sizesA(3);
  sizesA[0] = 1; sizesA[1] = 1; sizesA[2] = 1;
 
  std::vector<size_t> sizesB(3);
  sizesB[0] = 0; sizesB[1] = 1; sizesB[2] = 2;

  std::vector<size_t> sizesC(3);
  sizesC[0] = 2; sizesC[1] = 1; sizesC[2] = 0;

  std::vector<size_t> sizesD(3);
  sizesD[0] = 2; sizesD[1] = 2; sizesD[2] = 2;
  
  sp.addSizes(sizesA);
  sp.addSizes(sizesB);
  sp.addSizes(sizesC);
  sp.addSizes(sizesD);
  
  sp.getOffsets(sizesA, chunks);
  assert(chunks.size() == 3);
  assert(*(chunks[0]) == 4);
  assert(*(chunks[1]) == 9);
  assert(*(chunks[2]) == 14);

  sp.getOffsets(sizesB, chunks);
  assert(*(chunks[0]) == 4);
  assert(*(chunks[1]) == 8);
  assert(*(chunks[2]) == 12);

  sp.getOffsets(sizesC, chunks);
  assert(*(chunks[0]) == 2);
  assert(*(chunks[1]) == 7);
  assert(*(chunks[2]) == 12);

  sp.getOffsets(sizesD, chunks);
  assert(*(chunks[0]) == 0);
  assert(*(chunks[1]) == 5);
  assert(*(chunks[2]) == 10);

}

