CC = gcc
RM = rm
CFLAGS = -O -Wall -std=gnu11 -I../inc

MEM_TEST_OBJS = mem.o mem_test.o

COMPRESS_TEST_OBJS = compress.o compress_test.o quicksort.o hm_tree.o

BITS_TEST_OBJS = bits_test.o

RB_TEST_OBJS = rb_tree.o rb_tree_test.o

HM_TEST_OBJS = hm_tree.o quicksort.o hm_tree_test.o

BASE64_TEST_OBJS = base64.o base64_test.o bits.o mem.o compress.o hm_tree.o quicksort.o

.c.o:
	$(CC) -c $(CFLAGS) $*.c

all: compress_test bits_test rb_test hm_test base64_test

base64_test: $(BASE64_TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(BASE64_TEST_OBJS)

mem_test: $(MEM_TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(MEM_TEST_OBJS)

compress_test: $(COMPRESS_TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(COMPRESS_TEST_OBJS)

bits_test: $(BITS_TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(BITS_TEST_OBJS)

rb_test: $(RB_TEST_OBJS)
	$(CC) $(CLFAGS) -o $@ $(RB_TEST_OBJS)

hm_test: $(HM_TEST_OBJS)
	$(CC) $(CLFAGS) -o $@ $(HM_TEST_OBJS)

clean:
	$(RM) -f *.o compress_test bits_test rb_test hm_test base64_test mem_test
