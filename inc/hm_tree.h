#pragma once

struct HMNode {
	struct HMNode *left;
	struct HMNode *right;
};

typedef struct {
	int (*cmpHMNode)(const void *, const void *);
	struct HMNode *(*mergeHMTree)(struct HMNode *, struct HMNode *);
	struct HMNode **seeds;
	unsigned int seed_num;
} HMSeed, *HMSeedPtr;

typedef struct {
	struct HMNode *root;
} HMTree, *HMTreePtr;

HMSeed makeHMSeed(struct HMNode **, unsigned int, int (*)(const void *, const void *), 
	struct HMNode *(*)(struct HMNode *, struct HMNode *));
HMTree makeHMTree(HMSeed);
void destroyHMTree(HMTree, void (*)(struct HMNode *));
struct HMNode *getHMRoot(HMTree);
