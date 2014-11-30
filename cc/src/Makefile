CC = gcc

CXX = g++

RM = rm

CFLAGS = -Wall -std=c11 -I../inc

CXXFLAGS = -Wall -std=c++14 -I../inc

EXPLODE_TEST_OBJS = explode.o explode_test.o

.c.o:
	$(CC) -c $(CFLAGS) $*.c

.cxx.o:
	g++ -c $(CXXFLAGS) $*.cc

all: explode_test

explode_test: $(EXPLODE_TEST_OBJS)
	g++ $(CXXLFAGS) -o $@ $(EXPLODE_TEST_OBJS)

clean:
	$(RM) -f *.o explode_test
