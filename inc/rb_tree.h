/**************************************************************************
 * rb_tree.h                                                              * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 **************************************************************************/

#pragma once

#include <stdbool.h>

struct RBNode {
        unsigned long  rb_parent_color;
#define RB_RED          0
#define RB_BLACK        1
        struct RBNode *rb_right;
        struct RBNode *rb_left;
};

typedef struct {
        void            *(*getKey)(struct RBNode *); 
        int             (*cmpKey)(void *, void *); 
        struct RBNode  *(*makeNode)(void *); 
        void            (*freeNode)(struct RBNode *); 
        struct RBNode *rb_node;
} RBTree, *RBTreePtr;

bool		rbInsert(RBTreePtr, void *);
void		rbDelete(RBTreePtr, void *);
struct RBNode*	rbSearch(RBTree, void *);

struct 	RBNode* rbGetRoot(RBTree);
void 	rbSetReleaseFunc(RBTreePtr tree, void (*releaseFunc)(struct RBNode *));
RBTree 	makeRBTree(void *(*)(struct RBNode *), int (*)(void *, void *),		\
	struct RBNode *(*)(void *key),						\
	void    (*)(struct RBNode *));
void 	destroyRBTree(RBTree);
