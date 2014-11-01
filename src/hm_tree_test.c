#include "hm_tree.h"
#include "container.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct MyNode {
	int val;
	struct HMNode hm_node;
};

int
cmp_func(const void *p1, const void *p2)
{
	struct HMNode **node1 = (struct HMNode **)p1;
	struct HMNode **node2 = (struct HMNode **)p2;
	return container_of(*node2, struct MyNode, hm_node)->val - 
		container_of(*node1, struct MyNode, hm_node)->val;
}

struct HMNode *
mergeHMTree(struct HMNode *t1, struct HMNode *t2)
{
	struct MyNode *my_node = malloc(sizeof(struct MyNode));
	assert(my_node != NULL);
	my_node->val = container_of(t1, struct MyNode, hm_node)->val 
		+ container_of(t2, struct MyNode, hm_node)->val;
	my_node->hm_node.left = t1;
	my_node->hm_node.right = t2;
	return &my_node->hm_node;
}

void
dumpTree(struct HMNode *tree)
{
	if(tree->left == NULL && tree->right == NULL) {
		printf("<%d>\n", container_of(tree, struct MyNode, hm_node)->val);
		printf("return\n");
		return;
	}
	if(tree->left != NULL) {
		printf("go left\n");
		dumpTree(tree->left);
	}
	if(tree->right != NULL) {
		printf("go right\n");
		dumpTree(tree->right);
	}
	printf("return\n");
}

void
destroyNode(struct HMNode *node)
{
	if(node->left == NULL && node->right == NULL)
		return;
	free(container_of(node, struct MyNode, hm_node));
}

int
main(int argc, char *argv[])
{
	struct MyNode nodes[5] = { { 0 } };
	for(int i = 0; i < 5; i++) {
		nodes[i].val = i;
	}
	struct HMNode *hm_nodes[] = { &nodes[1].hm_node, &nodes[2].hm_node, 
		&nodes[3].hm_node, &nodes[4].hm_node, &nodes[0].hm_node };
	HMSeed seed = makeHMSeed(hm_nodes, 5, cmp_func, mergeHMTree);
	HMTree tree = makeHMTree(seed);	
	dumpTree(tree.root);
	destroyHMTree(tree, destroyNode);
	return 0;
}
