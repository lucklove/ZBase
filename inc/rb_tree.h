#pragma once

struct RBNode {
        struct RBNode* lchild, *rchild, *parent;
        int colour;
};

typedef struct {
        void            *(*getKey)(struct RBNode *); 
        int             (*cmpKey)(void *, void *); 
        struct RBNode  *(*makeNode)(void *); 
        void            (*swapKey)(struct RBNode *, struct RBNode *); 
        void            (*freeNode)(struct RBNode *); 
        struct RBNode *root;
} RBTree, *RBTreePtr;

void		rbInsert(RBTreePtr, void *);
void		rbDelete(RBTreePtr, void *);
struct RBNode*	rbSearch(RBTree, void *);

struct RBNode* getRBRoot(RBTree);

RBTree 	makeRBTree(void *(*)(struct RBNode *), int (*)(void *, void *),		\
	struct RBNode *(*)(void *key),						\
	void    (*)(struct RBNode *, struct RBNode *),				\
	void    (*)(struct RBNode *));
void 	destroyRBTree(RBTree);
