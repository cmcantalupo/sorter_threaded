// Quick sort implementation that can be used if built in std::sort()
// is not thread safe.
//
// C.M. Cantalupo 2011
// cmcantalupo@gmail.com

#ifndef quick_sort_hpp
#define quick_sort_hpp
#include <vector>

namespace SorterThreadedHelper {
  template <class type>
  void quick_sort(typename std::vector<type>::iterator begin, 
                  typename std::vector<type>::iterator end);

  template <class type>
  typename std::vector<type>::iterator 
  part(typename std::vector<type>::iterator begin, 
       typename std::vector<type>::iterator end, type value);

  template <class type>
  void ts_copy(typename std::vector<type>::iterator begin, 
               typename std::vector<type>::iterator end,
               typename std::vector<type>::iterator result);

  template <class type>
  typename std::vector<type>::iterator 
  part(typename std::vector<type>::iterator begin, 
       typename std::vector<type>::iterator end, type value) {
    // cribbed from the explaination of STL partition
    // http://www.cplusplus.com/reference/algorithm/partition/
    while (true) {
      while (begin != end && *begin < value) ++begin;
      if (begin == end--) break;
      while (begin != end && !(*end < value)) --end;
      if (begin == end) break;
      std::swap(*begin++, *end);
    }
    return begin;
  }

  // Just in case STL copy is not thread safe.  
  template <class type>
  void ts_copy(typename std::vector<type>::iterator begin, 
               typename std::vector<type>::iterator end,
               typename std::vector<type>::iterator result) {
    while (begin != end) {
      *result = *begin;
      ++result;
      ++begin;
    }
  }

  // If STL sort is not thread safe this can be used instead.  
  template <class type>
  void quick_sort(typename std::vector<type>::iterator begin, 
                  typename std::vector<type>::iterator end) {
    // Recursive implementation of the quick sort algorithm
    // If the vector has less than two elements return
    if (distance(begin, end) < 2){
      return;
    }
    typename std::vector<type>::iterator bound;
    type beginVal = *begin;
    bound = part(begin+1, end, beginVal);
    ts_copy<type>(begin + 1, bound, begin);
    *(bound-1) = beginVal;
    quick_sort<type>(begin, bound-1);
    quick_sort<type>(bound, end);
  }
}

#endif
