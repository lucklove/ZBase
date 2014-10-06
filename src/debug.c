#include "debug.h"
#include "config.h"
#include "mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct debug_t *d_list = NULL;
static int pth_number;
FILE *debug_file;

void
debug_init(int pth_num)
{
	debug_file = fopen(DEBUG_FILE, "w");
	assert(debug_file != NULL);
	Malloc(d_list, sizeof(struct debug_t) * pth_num);
	pth_number = pth_num;
}

void
debug_record(pthread_t pth, char *file, int line)
{
	for(int i = 0; i < pth_number; i++) {
		if(d_list[i].key == pth || d_list[i].key == 0) {
			d_list[i].key = pth;
			d_list[i].file = file;
			d_list[i].line = line;
			return;
		}
	}
}	

void
debug_stat()
{
	for(int i = 0; i < pth_number; i++) {
		if(d_list[i].key != 0) {
			printf("pth:%ld\tfile:%s\tline:%d\n",
				d_list[i].key, d_list[i].file, d_list[i].line);
		}
	}
}

void
debug_mem_new(char *file, int line, void *ptr)
{
	fprintf(debug_file, "alloc %p in file %s line %d\n", ptr, file, line);
	fflush(debug_file);
}

void
debug_mem_free(char *file, int line, void *ptr)
{
	fprintf(debug_file, "free %p in file %s line %d\n", ptr, file, line);
	fflush(debug_file);
}		
