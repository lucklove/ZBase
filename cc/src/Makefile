CC = clang

CXX = clang++

RM = rm

CFLAGS = -Wall -std=c11 -I../inc

CXXFLAGS = -Wall -std=c++14 -I../inc

LDFLAGS = -lcurl

EXPLODE_TEST_OBJS = explode_test.o

PARSE_TEST_OBJS = parse_test.o

CONCURRENCY_TEST_OBJS = concurrency_test.o concurrency.o

UTILITY_TEST_OBJS = utility_test.o

LOADFILE_TEST_OBJS = loadfile_test.o

MULTIINDEXMAP_TEST_OBJS = MultiIndexMap_test.o

ALGORITHM_TEST_OBJS = algorithm_test.o

EXCEPTION_TEST_OBJS = exception_test.o

RETVAL_TEST_OBJS = retval_test.o

REST_CLIENT_TEST_OBJS = RestClient.o RestClient_test.o

.c.o:
	$(CC) -c $(CFLAGS) $*.c

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $*.cc

all: explode_test parse_test concurrency_test utility_test loadfile_test multiindexmap_test  algorithm_test exception_test retval_test restclient_test

restclient_test: $(REST_CLIENT_TEST_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(REST_CLIENT_TEST_OBJS)

retval_test: $(RETVAL_TEST_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(RETVAL_TEST_OBJS)

exception_test: $(EXCEPTION_TEST_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(EXCEPTION_TEST_OBJS)

algorithm_test : $(ALGORITHM_TEST_OBJS) 
	 $(CXX) $(LDFLAGS) -o $@ $(ALGORITHM_TEST_OBJS)

multiindexmap_test: $(MULTIINDEXMAP_TEST_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(MULTIINDEXMAP_TEST_OBJS)

loadfile_test: $(LOADFILE_TEST_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(LOADFILE_TEST_OBJS)

utility_test: $(UTILITY_TEST_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(UTILITY_TEST_OBJS)

explode_test: $(EXPLODE_TEST_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(EXPLODE_TEST_OBJS)

parse_test: $(PARSE_TEST_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(PARSE_TEST_OBJS)

concurrency_test: $(CONCURRENCY_TEST_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(CONCURRENCY_TEST_OBJS) -lpthread
	
clean:
	$(RM) -f *.o *.txt explode_test parse_test concurrency_test utility_test loadfile_test multiindexmap_test algorithm_test exception_test retval_test restclient_test
