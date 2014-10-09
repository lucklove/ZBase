#include <stdio.h>
#include <stdlib.h>
#include "rb_tree.h"
#include "container.h"
#include <string.h>

struct MyNode {
        char *key;
        struct RBNode node;
};

void
swapKey(struct RBNode *src, struct RBNode *dst)
{
        char *tmp = container_of(dst, struct MyNode, node)->key;
        container_of(dst, struct MyNode, node)->key = container_of(src, struct MyNode, node)->key;
        container_of(src, struct MyNode, node)->key = tmp;
}

void *
getKey(struct RBNode *p)
{
        return container_of(p, struct MyNode, node)->key;
}

int
cmpKey(void *p1, void *p2)
{
        return strcmp(p1, p2);
}

struct RBNode *
makeNode(void *key)
{
        struct MyNode *tmp = malloc(sizeof(struct MyNode));
        tmp->key = key;
        return &tmp->node;
}

void
freeNode(struct RBNode *node)
{
	printf("free %s\n", container_of(node, struct MyNode, node)->key);
        free(container_of(node, struct MyNode, node));
}

void 
show_rb_tree(struct RBNode* node){
        if(node == NULL)
                return;
        printf("(%s,%s)\n", container_of(node, struct MyNode, node)->key, node->colour == 1 ? "BLACK" : "RED");
        if(node->lchild != NULL){
                printf("%s left start:\n", container_of(node, struct MyNode, node)->key);
                show_rb_tree(node->lchild);
                printf("%s left end:\n", container_of(node, struct MyNode, node)->key);
        }
        if(node->rchild != NULL){
                printf("%s right start:\n", container_of(node, struct MyNode, node)->key);
                show_rb_tree(node->rchild);
                printf("%s right end:\n", container_of(node, struct MyNode, node)->key);
        }
}
 
int 
main(int argc, char *argv[]){
//     	RBTree t = makeRBTree(getKey, cmpKey, makeNode, swapKey, freeNode);
     	RBTree t = makeRBTree(getKey, cmpKey, makeNode, swapKey, freeNode);
	rb_insert(&t, "hello");
	rb_insert(&t, "wawa");
	rb_insert(&t, "nini");
	rb_insert(&t, "wowo");
	rb_insert(&t, "cao");
	rb_insert(&t, "nima");
	rb_insert(&t, "hello");
     	rb_delete(&t, "hello");
//     	rb_delete(&t, "hello");
     	show_rb_tree(t.root);
     	struct RBNode *node = rb_search(t, "hello");
     	if(node != NULL) {
     		printf("found\n");
        	printf("key:%s\n", container_of(node, struct MyNode, node)->key);
     	}
	destroyRBTree(t);
        return 0;
}

