CC = gcc
RM = rm
CFLAGS = -O -Wall -std=gnu11 -I../inc

COMPRESS_TEST_OBJS = compress.o compress_test.o quicksort.o hm_tree.o

BITS_TEST_OBJS = bits_test.o

RB_TEST_OBJS = rb_tree.o rb_tree_test.o

HM_TEST_OBJS = hm_tree.o quicksort.o hm_tree_test.o

.c.o:
	$(CC) -c $(CFLAGS) $*.c

all: compress_test bits_test rb_test hm_test

compress_test: $(COMPRESS_TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(COMPRESS_TEST_OBJS)

bits_test: $(BITS_TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(BITS_TEST_OBJS)

rb_test: $(RB_TEST_OBJS)
	$(CC) $(CLFAGS) -o $@ $(RB_TEST_OBJS)

hm_test: $(HM_TEST_OBJS)
	$(CC) $(CLFAGS) -o $@ $(HM_TEST_OBJS)

clean:
	$(RM) -f *.o compress_test bits_test rb_test hm_test
