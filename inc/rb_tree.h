#ifndef RB_TREE_H
#define RB_TREE_H

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

void		rb_insert(RBTreePtr, void *);
void		rb_delete(RBTreePtr, void *);
struct RBNode*	rb_search(RBTree, void *);

RBTree 	makeRBTree(void *(*)(struct RBNode *), int (*)(void *, void *),		\
	struct RBNode *(*)(void *key),						\
	void    (*)(struct RBNode *, struct RBNode *),				\
	void    (*)(struct RBNode *));
void 	destroyRBTree(RBTree);

#endif
