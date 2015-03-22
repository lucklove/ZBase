//Huffman tree
#include "hm_tree.h"
#include "sort.h"
#include <assert.h>

#ifndef NULL
#define NULL ((void *)0)
#endif


inline struct HMNode *
getHMRoot(HMTree tree)
{
	return tree.root;
}

HMSeed
makeHMSeed(struct HMNode **seeds, unsigned int seed_num,
	int (*cmpHMNode)(const void *, const void *),
	struct HMNode *(*mergeHMTree)(struct HMNode *, struct HMNode *))
{
	return (HMSeed){ cmpHMNode, mergeHMTree, seeds, seed_num };
}

HMTree
makeHMTree(HMSeed seed)
{
	while(seed.seed_num > 1) {
		quickSort(seed.seeds, sizeof(struct HMNode *), seed.seed_num,	seed.cmpHMNode);
		--seed.seed_num;
		seed.seeds[seed.seed_num-1] = seed.mergeHMTree(seed.seeds[seed.seed_num-1], seed.seeds[seed.seed_num]);
	}
	HMTree new_tree = { seed.seeds[0] };
	return new_tree;
}

static void
destroyHMNode(struct HMNode *node, void (*destroyNode)(struct HMNode *))
{
	if(node->left == NULL && node->right == NULL) {
		destroyNode(node);
		return;
	}
	if(node->left != NULL)
		destroyHMNode(node->left, destroyNode);
	if(node->right != NULL)
		destroyHMNode(node->right, destroyNode);
	destroyNode(node);
}

void
destroyHMTree(HMTree tree, void (*destroyNode)(struct HMNode *))
{
	struct HMNode *root = tree.root;
	if(root == NULL)
		return;
	destroyHMNode(root, destroyNode);
}		
