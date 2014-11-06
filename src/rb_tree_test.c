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
	printf("%s\n", container_of(node, struct MyNode, node)->key);
        if(node->rb_left != NULL){
                printf("%s left start:\n", container_of(node, struct MyNode, node)->key);
                show_rb_tree(node->rb_left);
                printf("%s left end:\n", container_of(node, struct MyNode, node)->key);
        }
        if(node->rb_right != NULL){
                printf("%s right start:\n", container_of(node, struct MyNode, node)->key);
                show_rb_tree(node->rb_right);
                printf("%s right end:\n", container_of(node, struct MyNode, node)->key);
        }
}

#define try_find(_key) 										\
do {												\
	struct RBNode *node = rbSearch(t, _key); 						\
	if(node == NULL) {									\
		printf("%s not found\n", _key);							\
	} else {										\
		printf("%s found\n", container_of(node, struct MyNode, node)->key);		\
	}											\
} while(0)
	
int 
main(int argc, char *argv[]){
     	RBTree t = makeRBTree(getKey, cmpKey, makeNode, swapKey, freeNode);
	rbInsert(&t, "hello");
	rbInsert(&t, "haha");
	rbInsert(&t, "xixi");
	rbInsert(&t, "mimi");
	rbInsert(&t, "gugu");
	rbInsert(&t, "baba");
	rbInsert(&t, "lala");
	rbInsert(&t, "kaka");
	rbInsert(&t, "jiji");
     	show_rb_tree(t.rb_node);
	try_find("gugu");
	try_find("haha");
     	rbDelete(&t, "haha");
     	printf("root %p\n", t.rb_node);
	destroyRBTree(t);
        return 0;
}

