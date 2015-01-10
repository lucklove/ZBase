CC = gcc

CXX = clang++

RM = rm

CFLAGS = -Wall -std=c11 -I../inc

CXXFLAGS = -Wall -std=c++14 -I../inc

EXPLODE_TEST_OBJS = explode_test.o

PARSE_TEST_OBJS = parse_test.o

CONCURRENCY_TEST_OBJS = concurrency_test.o concurrency.o

UTILITY_TEST_OBJS = utility_test.o

LOADFILE_TEST_OBJS = loadfile_test.o

MULTIINDEXMAP_TEST_OBJS = MultiIndexMap_test.o

ALGORITHM_TEST_OBJS = algorithm_test.o

EXCEPTION_TEST_OBJS = exception_test.o

.c.o:
	$(CC) -c $(CFLAGS) $*.c

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $*.cc

all: explode_test parse_test concurrency_test utility_test loadfile_test multiindexmap_test  algorithm_test exception_test

exception_test: $(EXCEPTION_TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(EXCEPTION_TEST_OBJS)

algorithm_test : $(ALGORITHM_TEST_OBJS) 
	 $(CXX) $(CXXFLAGS) -o $@ $(ALGORITHM_TEST_OBJS)

multiindexmap_test: $(MULTIINDEXMAP_TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(MULTIINDEXMAP_TEST_OBJS)

loadfile_test: $(LOADFILE_TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(LOADFILE_TEST_OBJS)

utility_test: $(UTILITY_TEST_OBJS)
	$(CXX) $(CXXLFAGS) -o $@ $(UTILITY_TEST_OBJS)

explode_test: $(EXPLODE_TEST_OBJS)
	$(CXX) $(CXXLFAGS) -o $@ $(EXPLODE_TEST_OBJS)

parse_test: $(PARSE_TEST_OBJS)
	$(CXX) $(CXXLFAGS) -o $@ $(PARSE_TEST_OBJS)

concurrency_test: $(CONCURRENCY_TEST_OBJS)
	$(CXX) $(CXXLFAGS) -o $@ $(CONCURRENCY_TEST_OBJS) -lpthread
	
clean:
	$(RM) -f *.o *.txt explode_test parse_test concurrency_test utility_test loadfile_test multiindexmap_test algorithm_test exception_test
