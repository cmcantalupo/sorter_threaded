CC=g++
CPPFLAGS=-O3 -fpic -fopenmp -DSTL_SORT_THREAD_SAFE
LDFLAGS=-fopenmp

all : partition_wall_test partition_test splinter_test quick_sort_test sorter_threaded_test 

clean :
	rm -f partition_wall_test partition_wall_test.o partition_test partition_test.o splinter_test splinter_test.o quick_sort_test quick_sort_test.o sorter_threaded_test sorter_threaded_test.o 

partition_wall_test : src/partition_wall.hpp src/partition_wall_test.cpp
	${CC} ${CPPFLAGS} src/partition_wall_test.cpp -o partition_wall_test
	./partition_wall_test

partition_test : src/partition.hpp src/partition_test.cpp
	${CC} ${CPPFLAGS} src/partition_test.cpp -o partition_test
	./partition_test

splinter_test : src/splinter.hpp src/splinter_test.cpp 
	${CC} ${CPPFLAGS} src/splinter_test.cpp -o splinter_test
	./splinter_test

quick_sort_test : src/quick_sort.hpp
	${CC} ${CPPFLAGS} src/quick_sort_test.cpp -o quick_sort_test
	./quick_sort_test

sorter_threaded_test : src/sorter_threaded.hpp src/sorter_threaded_test.cpp
	${CC} ${CPPFLAGS} src/sorter_threaded_test.cpp -o sorter_threaded_test
	./sorter_threaded_test

#