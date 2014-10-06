#ifndef LIST_H
#define LIST_H

#include "container.h"

//#define LIST_POISON1 ((void *)0x00100100)
//#define LIST_POISON2 ((void *)0x00200200)
#define LIST_POISON1 NULL
#define LIST_POISON2 NULL

struct list_node {
	struct list_node *prev;
	struct list_node *next;
};

void list_add(struct list_node *, struct list_node *);
void list_del(struct list_node *);

#endif
	
