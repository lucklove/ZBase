compress_test: compress_test.c object.c quicksort.c hm_tree.c rb_tree.c
	gcc -Wall -o test -std=gnu11 -I../inc compress_test.c compress.c quicksort.c hm_tree.c rb_tree.c
bits_test: ../inc/bits.h bits_test.c
	gcc -Wall -o test -std=gnu11 -I../inc bits_test.c
rb_test: rb_tree.c rb_tree_test.c
	gcc -Wall -o test -std=gnu11 -I../inc rb_tree.c rb_tree_test.c
hm_test: hm_tree.c quicksort.c hm_tree_test.c
	gcc -Wall -o test -std=gnu11 -I../inc hm_tree.c quicksort.c hm_tree_test.c
