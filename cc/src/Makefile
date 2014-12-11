CC = gcc

CXX = g++

RM = rm

CFLAGS = -Wall -std=c11 -I../inc

CXXFLAGS = -Wall -std=c++14 -I../inc

EXPLODE_TEST_OBJS = explode_test.o

PARSE_TEST_OBJS = parse_test.o

CONCURRENCY_TEST_OBJS = concurrency_test.o concurrency.o

.c.o:
	$(CC) -c $(CFLAGS) $*.c

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $*.cc

all: explode_test parse_test concurrency_test

explode_test: $(EXPLODE_TEST_OBJS)
	$(CXX) $(CXXLFAGS) -o $@ $(EXPLODE_TEST_OBJS)

parse_test: $(PARSE_TEST_OBJS)
	$(CXX) $(CXXLFAGS) -o $@ $(PARSE_TEST_OBJS)

concurrency_test: $(CONCURRENCY_TEST_OBJS)
	$(CXX) $(CXXLFAGS) -o $@ $(CONCURRENCY_TEST_OBJS) -lpthread
	
clean:
	$(RM) -f *.o explode_test parse_test concurrency_test
