#include "list.h"

static inline void
__list_add(struct list_node *new, struct list_node *prev, struct list_node *next)
{
	if(next == NULL) {
		new->next = new;
	} else {
		new->next = next;
		next->prev = new;
	}
	if(prev == NULL) {
		new->prev = new;
	} else {
		new->prev = prev;
		prev->next = new;
	}
}

void
list_add(struct list_node *new, struct list_node *node)
{
	if(node == NULL) {
		__list_add(new, NULL, NULL);
	} else {	
		__list_add(new, node, node->next);
	}
}

static inline void
__list_del(struct list_node *prev, struct list_node *next)
{
	next->prev = prev;
	prev->next = next;
}

void
list_del(struct list_node *dest)
{
	__list_del(dest->prev, dest->next);
	dest->prev = LIST_POISON1;
	dest->next = LIST_POISON2;
}	
