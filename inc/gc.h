#ifndef	GC_H
#define GC_H

extern void *stack_top;
extern void **trace_list;
extern int  list_size;

void *gc_malloc(unsigned int);

#define gc_init(arg) do { stack_top = &arg; } while(0)
//#define gc_trace(ptr) do { trace_list_add(&p); } while(0)

#endif
