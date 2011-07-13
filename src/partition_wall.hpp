// Class used in partition map.  It wraps the type of the vector to be
// sorted with a bool value that determines if the partition is after
// the last pivot or not.
//
// C.M. Cantalupo 2011
// cmcantalupo@gmail.com

#ifndef st_partition_wall_hpp
#define st_partition_wall_hpp

#include <stack>

namespace SorterThreadedHelper {

  template <class type>
  class PartitionWall {
    public:
      PartitionWall(const type& pivot, const bool &isEnd = false);
      PartitionWall();
      void set(const type &pivot, const bool &isEnd = false);
      template <class ftype>
      friend bool operator< (const PartitionWall<ftype>& l, 
                             const PartitionWall<ftype>& r);
    private:
      bool isEnd_;
      type pivot_;
  };

  template <class type>
  PartitionWall<type>::PartitionWall(const type& pivot, const bool &isEnd) :
    pivot_(pivot), isEnd_(isEnd) {}

  template <class type>
  PartitionWall<type>::PartitionWall() :
    pivot_(), isEnd_(true) {}

  template <class type>
  bool operator< (const PartitionWall<type>& l, 
                  const PartitionWall<type>& r) {
    if (l.isEnd_) return false;
    if (r.isEnd_) return true;
    return l.pivot_ < r.pivot_;     
  }

  template <class type>
  void PartitionWall<type>::set(const type &pivot, const bool &isEnd) {
    pivot_ = pivot;
    isEnd_ = isEnd;
  }
}

#endif
