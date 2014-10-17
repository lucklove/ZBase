#include "rb_tree.h"
#define NULL ((void *)0)
typedef enum {
	RED = 0,
	BLACK = 1
} RBColor;

RBTree
makeRBTree(void *(*getKey)(struct RBNode *), int (*cmpKey)(void *, void *), 
	struct RBNode *(*makeNode)(void *key), 
	void	(*swapKey)(struct RBNode *, struct RBNode *),
	void	(*freeNode)(struct RBNode *))
{
	return (RBTree){ getKey, cmpKey, makeNode, swapKey, freeNode, NULL };
}

static void
destroyTree(struct RBNode *tree, void (*freeFunc)(struct RBNode *))
{
	if(tree->lchild != NULL)
		destroyTree(tree->lchild, freeFunc);
	if(tree->rchild != NULL)
		destroyTree(tree->rchild, freeFunc);
	freeFunc(tree);
}

void
destroyRBTree(RBTree tree)
{
	if(tree.freeNode != NULL)
		destroyTree(tree.root, tree.freeNode);
}

struct RBNode *
rbGetRoot(RBTree tree)
{
	return tree.root;
}
	
static struct RBNode* 
clock_wise(RBTreePtr tree_ptr, struct RBNode* node)
{
	if(node == NULL || node->lchild == NULL)
      		return NULL;
      	struct RBNode *rb_1 = node, *rb_2=node->lchild, *rb_3=node->lchild->rchild;
      	if(rb_1->parent != NULL){
      		if(rb_1->parent->lchild == rb_1) {
          		rb_1->parent->lchild = rb_2;
		} else {
          		rb_1->parent->rchild = rb_2;
		}
      	} else if (rb_1 == tree_ptr->root){
      		tree_ptr->root = rb_2;
      	}
      	rb_2->parent = rb_1->parent;
  	rb_1->parent = rb_2;
	rb_2->rchild = rb_1;
	rb_1->lchild = rb_3;
	if(rb_3 != NULL)
		rb_3->parent = rb_1;
	return rb_2;    
}

static struct RBNode* 
counter_clock_wise(RBTreePtr tree_ptr, struct RBNode* node)
{
	if(node == NULL || node->rchild == NULL)
		return NULL;

	struct RBNode *rb_1=node, *rb_2=node->rchild, *rb_3=node->rchild->lchild;
	if(rb_1->parent != NULL){
	if(rb_1->parent->lchild == rb_1)
		rb_1->parent->lchild = rb_2;
	else 
		rb_1->parent->rchild = rb_2;
	}
	else if(rb_1 == tree_ptr->root){
		tree_ptr->root = rb_2;
	}
	rb_2->parent = rb_1->parent;

	rb_1->parent = rb_2;
	rb_2->lchild = rb_1;

	rb_1->rchild = rb_3;
	if(rb_3 != NULL)
		rb_3->parent = rb_1;

	return rb_2;
}

struct RBNode* 
rbSearch(RBTree tree, void *key)
{
	struct RBNode *p = tree.root;
	while(p != NULL){
		if(tree.cmpKey(key, tree.getKey(p)) < 0)
			p = p->lchild;
		else if(tree.cmpKey(key, tree.getKey(p)) > 0)
			p = p->rchild;
		else
			break;
	}
	return p;
}

static struct RBNode *
newNode(RBTreePtr tree_ptr, struct RBNode* parent, void *key)
{
	struct RBNode *tmp = tree_ptr->makeNode(key);
	tmp->colour = RED;
        tmp->parent = parent;
        tmp->lchild = tmp->rchild = NULL;
	return tmp;
}

void 
rbInsert(RBTreePtr tree_ptr, void *key)
{
	struct RBNode *p= tree_ptr->root, *q=NULL;

	if(tree_ptr->root == NULL) {
		tree_ptr->root = newNode(tree_ptr, NULL, key);
		tree_ptr->root->colour = BLACK;
		return;
	}
 
     	while(p != NULL){
     		q = p;
		if(tree_ptr->cmpKey(key, tree_ptr->getKey(p)) < 0)
        		p = p->lchild;
		else if(tree_ptr->cmpKey(key, tree_ptr->getKey(p)) > 0)
        		p = p->rchild;
	     	else return;
     	}
	if(tree_ptr->cmpKey(key, tree_ptr->getKey(q)) < 0) {
     		q->lchild = newNode(tree_ptr, q, key);
     	} else {
     		q->rchild = newNode(tree_ptr, q, key);
	}
  
     	while(q != NULL && q->colour == RED){
     		p = q->parent;//p won't null, or BUG.
 		if(p->lchild == q){
         		if(q->rchild != NULL && q->rchild->colour == RED)
         			counter_clock_wise(tree_ptr, q);        
         		q = clock_wise(tree_ptr, p);
         		q->lchild->colour = BLACK;
     		} else {
         		if(q->lchild != NULL && q->lchild->colour == RED)
         			clock_wise(tree_ptr, q);
         		q = counter_clock_wise(tree_ptr, p);
         		q->rchild->colour = BLACK;
     		}
 		q = q->parent;
     	}
     	tree_ptr->root->colour = BLACK;
}
 
void 
rbDelete(RBTreePtr tree_ptr, void *key) 
{
	struct RBNode *v = rbSearch(*tree_ptr, key), *u, *p, *c, *b;
	if(v == NULL) 
		return;
 	u = v;
	if(v->lchild != NULL && v->rchild != NULL){
     		u = v->rchild;
     		while(u->lchild != NULL){
         		u = u->lchild;
     		}
     		tree_ptr->swapKey(u, v);
     	}
     	//u is the node to free.
     	if(u->lchild != NULL) {
     		c = u->lchild;
	} else { 
     		c = u->rchild;
	}
     	p = u->parent;
     	if(p != NULL){
     		//remove u from rb_tree.
     		if(p->lchild == u) {
         		p->lchild = c;
		} else {
         		p->rchild = c;
		}
     	} else {
     		//u is root.
     		tree_ptr->root = c;
		if(tree_ptr->freeNode != NULL)
     			tree_ptr->freeNode(u);
     		return;
     	}
     	//u is not root and u is RED, this will not unbalance.
     	if(u->colour == RED) {
		if(tree_ptr->freeNode != NULL)
     			tree_ptr->freeNode(u);
     		return;
     	}
	if(tree_ptr->freeNode != NULL)
     		tree_ptr->freeNode(u);
     	u = c;
     	//u is the first node to balance.
     	while(u != tree_ptr->root){
     		if(u != NULL && u->colour == RED){
         	//if u is RED, change it to BLACK can finsh.
         		u->colour = BLACK;
         		return;
     		}
 
     		if(u == p->lchild) {
         		b = p->rchild;
     		} else {
         		b = p->lchild;
 		}
     		//b is borther of u. b can't be null, or the rb_tree is must not balance.
     		if(b->colour == BLACK){
         		//If b's son is RED, rotate the node.
         		if(b->lchild != NULL && b->lchild->colour == RED){
         			if(u == p->lchild){
             				b = clock_wise(tree_ptr, b);
             				b->colour = BLACK;
             				b->rchild->colour = RED;
 
             				p = counter_clock_wise(tree_ptr, p);
             				p->colour = p->lchild->colour;
             				p->lchild->colour = BLACK;
             				p->rchild->colour = BLACK;
         			} else {
             				p = clock_wise(tree_ptr, p);
             				p->colour = p->rchild->colour;
             				p->rchild->colour = BLACK;
             				p->lchild->colour = BLACK;
         			}
         
         			return;
         		} else if(b->rchild != NULL && b->rchild->colour == RED) {
         			if(u == p->rchild){
             				b = counter_clock_wise(tree_ptr, b);
             				b->colour = BLACK;
             				b->lchild->colour = RED;
 
             				p = clock_wise(tree_ptr, p);
             				p->colour = p->rchild->colour;
             				p->rchild->colour = BLACK;
             				p->lchild->colour = BLACK;
         			} else {
             				p = counter_clock_wise(tree_ptr, p);
             				p->colour = p->lchild->colour;
             				p->lchild->colour = BLACK;
             				p->rchild->colour = BLACK;
         			}        
         			return;
         		} else { //if b's sons are BLACK, make b RED and move up u.
         			b->colour = RED;
         			u = p;
         			p = u->parent;
         			continue;
         		}
     		} else {
         		if(u == p->lchild) {
         			p = counter_clock_wise(tree_ptr, p);
         			p->colour = BLACK;
         			p->lchild->colour = RED;
         			p = p->lchild;
         		} else {
         			p = clock_wise(tree_ptr, p);
         			p->colour = BLACK;
         			p->rchild->colour = RED;
         			p = p->rchild;
         		}
     		}
     	}
     	tree_ptr->root->colour = BLACK;
}
