// Exception class for SorterThreaded.  
//
// C.M. Cantalupo 2011
// cmcantalupo@gmail.com

#ifndef sorter_threaded_exception_hpp
#define sorter_threaded_exception_hpp

struct SorterThreadedException : std::exception {
  enum Error {SplinterOrder = 1,
              SplinterSize = 2};
  inline SorterThreadedException(Error code) : error(code) {} 
  const Error error;
};

#endif
