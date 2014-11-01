CC = gcc
RM = rm
CFLAGS = -Wall -std=c11 -I../inc -ltcmalloc

MEM_TEST_OBJS = mem.o mem_test.o rb_tree.o

COMPRESS_TEST_OBJS = compress.o compress_test.o quicksort.o hm_tree.o

BITS_TEST_OBJS = bits_test.o bits.o

RB_TEST_OBJS = rb_tree.o rb_tree_test.o

HM_TEST_OBJS = hm_tree.o quicksort.o hm_tree_test.o

BASE64_TEST_OBJS = base64.o base64_test.o bits.o mem.o compress.o hm_tree.o quicksort.o rb_tree.o

FSM_TEST_OBJS = fsm.o fsm_test.o mem.o rb_tree.o

RANDOM_TEST_OBJS = random.o random_test.o

JSON_TEST_OBJS = json.o json_test.o mem.o rb_tree.o

REF_TEST_OBJS = ref.o ref_test.o

OPT_TEST_OBJS = opt_test.o

ZOBJ_TEST_OBJS = zobject.o zobj_test.o mem.o rb_tree.o

MATRIX_TEST_OBJS = matrix.o matrix_test.o mem.o

SORT_TEST_OBJS = sort_test.o quicksort.o stacksort.o

.c.o:
	$(CC) -c $(CFLAGS) $*.c

all: compress_test bits_test rb_test hm_test base64_test random_test fsm_test mem_test json_test ref_test opt_test matrix_test zobj_test sort_test

sort_test: $(SORT_TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(SORT_TEST_OBJS)

matrix_test: $(MATRIX_TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(MATRIX_TEST_OBJS)

zobj_test: $(ZOBJ_TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(ZOBJ_TEST_OBJS)

opt_test: $(OPT_TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(OPT_TEST_OBJS)

ref_test: $(REF_TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(REF_TEST_OBJS)

json_test: $(JSON_TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(JSON_TEST_OBJS)

sha1_test: $(SHA1_TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(SHA1_TEST_OBJS)

random_test: $(RANDOM_TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(RANDOM_TEST_OBJS)

fsm_test: $(FSM_TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(FSM_TEST_OBJS)

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
	$(RM) -f *.o compress_test bits_test rb_test hm_test base64_test mem_test fsm_test json_test random_test ref_test opt_test zobj_test matrix_test sort_test
