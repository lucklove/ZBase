/**************************************************************************
 * rb_tree.h                                                              * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 * implemented in rb_tree.c						  *
 **************************************************************************/

#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * \brief Node in red black tree. 
 * \note User should make this at least on of their own struct's member.
 * \example
 * 	struct MyStruct {
 * 		void *mydata;
 * 		struct RBNode rb_node;
 * 	};
 */
struct RBNode {
	/** point to parent, the lowest 2 bits record color. */
        unsigned long  rb_parent_color;
#define RB_RED          0
#define RB_BLACK        1
        struct RBNode *rb_right;
        struct RBNode *rb_left;
};

/** record operation functions provided by user. */
typedef struct {
	/**
 	 * \brief Get the compare key in user's struct.
 	 * \example
 	 * 	void *
 	 * 	getKey(struct RBNode *node)
 	 * 	{
 	 * 		return container_of(node, struct MyStruct, rb_node)->mydata;
 	 * 	}
 	 */
        void            *(*getKey)(struct RBNode *); 	/**< get the key in user's struct */
	/** 
 	 * Compare two key, if the first param > the second, return a int val which > 0,
 	 * if the first param < the second, return a val which < 0, otherwise, 0.
 	 */ 
        int             (*cmpKey)(void *, void *);
	/**
 	 * \brief Make a node and return the rb_node address in the node.
 	 * \example
 	 * 	struct RBNode makeNode(void *key)
 	 * 	{
 	 * 		struct MyStruct *p = malloc(sizeof(struct MyStruct);
 	 * 		p->data = key;
 	 * 		return &p->rb_node;
 	 * 	}
 	 */  
        struct RBNode  *(*makeNode)(void *); 
	/**
 	 * \brief Release a node.
 	 * \note If you do not want to do anything when release a node, set this to NULL.
 	 * \example
 	 * 	void
 	 * 	releaseMyNode(struct RBNode *node)
 	 * 	{
 	 * 		free(container_of(node, struct MyStruct, rb_node)->data);
 	 * 		free(container_of(node, struct MyStruct, rb_node));
 	 * 	}
 	 */ 	
        void            (*freeNode)(struct RBNode *); 
	/** the root of red black tree. */
        struct RBNode *rb_node;
} RBTree, *RBTreePtr;

/** type of freeNode in RBTree struct. */
typedef void (*RBReleaseFunc)(struct RBNode *);

/**
 * \brief Insert a node to red black tree.
 * \param tree A pointer point to RBTree which you want to operat with.
 * \param key The key of the node you want to insert.
 * \return 
 * 	If success, true, else (for example, the same key is already in
 * 	the same tree), 0.
 */
bool		rbInsert(RBTreePtr tree, void *key);

/** delete a node whose key is the same of the param key. */
void		rbDelete(RBTreePtr tree, void *key);

/**
 * \brief 
 * 	Search the tree to find the node whose key is the same with 
 * 	the param key.
 * \param tree The tree you want to find in.
 * \return 
 * 	If found, a pointer point to the node's "rb_node" member who
 * 	has that key, else, NULL will be returned.
 */ 	
struct RBNode*	rbSearch(RBTree tree, void *key);

/** get the root of a red black tree. */
struct 	RBNode* rbGetRoot(RBTree);

/**
 * \brief Set the release function in the tree.
 * \return The old release function.
 */ 
RBReleaseFunc rbSetReleaseFunc(RBTreePtr tree, void (*releaseFunc)(struct RBNode *));

/**
 * \brief Make a RBTree type with given params.
 * \param getKey To set the getKey function pointer in RBTree.
 * \param cmpKey To set the cmpKey function pointer in RBTree.
 * \param makeNode To set the makeNode function pointer in RBTree.
 * \param freeNode To set the freeNode function pointer in RBTree.
 * \see RBTree.
 */ 
RBTree 	makeRBTree(void *(*getKey)(struct RBNode *), int (*cmpKey)(void *, void *),	\
	struct RBNode *(*makeNode)(void *key),	void (*freeNode)(struct RBNode *));

/**
 * Rlease RBTree's resource, the release function 
 * of user will be called(if have).
 */
void 	destroyRBTree(RBTree);

#ifdef __cplusplus
}
#endif
