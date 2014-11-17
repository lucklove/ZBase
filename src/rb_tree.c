#include "rb_tree.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

#define rb_parent(r)   ((struct RBNode *)((r)->rb_parent_color & ~3))
#define rb_color(r)   ((r)->rb_parent_color & 1)
#define rb_is_red(r)   (!rb_color(r))
#define rb_is_black(r) rb_color(r)
#define rb_set_red(r)  do { (r)->rb_parent_color &= ~1; } while (0)
#define rb_set_black(r)  do { (r)->rb_parent_color |= 1; } while (0)

typedef void (*rb_augment_f)(struct RBNode *node, void *data);

static inline void rb_set_parent(struct RBNode *rb, struct RBNode *p) 
{
        rb->rb_parent_color = (rb->rb_parent_color & 3) | (unsigned long)p;
}

static inline void rb_set_color(struct RBNode *rb, int color)
{
        rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}

static inline void rb_link_node(struct RBNode * node, struct RBNode * parent,
                                struct RBNode ** rb_link)
{
        node->rb_parent_color = (unsigned long )parent;
        node->rb_left = node->rb_right = NULL;

        *rb_link = node;
}

static void __rb_rotate_left(struct RBNode *node, RBTreePtr root)
{
	struct RBNode *right = node->rb_right;
	struct RBNode *parent = rb_parent(node);

	if ((node->rb_right = right->rb_left))
		rb_set_parent(right->rb_left, node);
	right->rb_left = node;

	rb_set_parent(right, parent);

	if (parent)
	{
		if (node == parent->rb_left)
			parent->rb_left = right;
		else
			parent->rb_right = right;
	}
	else
		root->rb_node = right;
	rb_set_parent(node, right);
}

static void __rb_rotate_right(struct RBNode *node, RBTreePtr root)
{
	struct RBNode *left = node->rb_left;
	struct RBNode *parent = rb_parent(node);

	if ((node->rb_left = left->rb_right))
		rb_set_parent(left->rb_right, node);
	left->rb_right = node;

	rb_set_parent(left, parent);

	if (parent)
	{
		if (node == parent->rb_right)
			parent->rb_right = left;
		else
			parent->rb_left = left;
	}
	else
		root->rb_node = left;
	rb_set_parent(node, left);
}

void rb_insert_color(struct RBNode *node, RBTreePtr root)
{
	struct RBNode *parent, *gparent;

	while ((parent = rb_parent(node)) && rb_is_red(parent))
	{
		gparent = rb_parent(parent);

		if (parent == gparent->rb_left)
		{
			{
				register struct RBNode *uncle = gparent->rb_right;
				if (uncle && rb_is_red(uncle))
				{
					rb_set_black(uncle);
					rb_set_black(parent);
					rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			if (parent->rb_right == node)
			{
				register struct RBNode *tmp;
				__rb_rotate_left(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			rb_set_black(parent);
			rb_set_red(gparent);
			__rb_rotate_right(gparent, root);
		} else {
			{
				register struct RBNode *uncle = gparent->rb_left;
				if (uncle && rb_is_red(uncle))
				{
					rb_set_black(uncle);
					rb_set_black(parent);
					rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			if (parent->rb_left == node)
			{
				register struct RBNode *tmp;
				__rb_rotate_right(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			rb_set_black(parent);
			rb_set_red(gparent);
			__rb_rotate_left(gparent, root);
		}
	}

	rb_set_black(root->rb_node);
}

static void __rb_erase_color(struct RBNode *node, struct RBNode *parent,
			     RBTreePtr root)
{
	struct RBNode *other;

	while ((!node || rb_is_black(node)) && node != root->rb_node)
	{
		if (parent->rb_left == node)
		{
			other = parent->rb_right;
			if (rb_is_red(other))
			{
				rb_set_black(other);
				rb_set_red(parent);
				__rb_rotate_left(parent, root);
				other = parent->rb_right;
			}
			if ((!other->rb_left || rb_is_black(other->rb_left)) &&
			    (!other->rb_right || rb_is_black(other->rb_right)))
			{
				rb_set_red(other);
				node = parent;
				parent = rb_parent(node);
			}
			else
			{
				if (!other->rb_right || rb_is_black(other->rb_right))
				{
					rb_set_black(other->rb_left);
					rb_set_red(other);
					__rb_rotate_right(other, root);
					other = parent->rb_right;
				}
				rb_set_color(other, rb_color(parent));
				rb_set_black(parent);
				rb_set_black(other->rb_right);
				__rb_rotate_left(parent, root);
				node = root->rb_node;
				break;
			}
		}
		else
		{
			other = parent->rb_left;
			if (rb_is_red(other))
			{
				rb_set_black(other);
				rb_set_red(parent);
				__rb_rotate_right(parent, root);
				other = parent->rb_left;
			}
			if ((!other->rb_left || rb_is_black(other->rb_left)) &&
			    (!other->rb_right || rb_is_black(other->rb_right)))
			{
				rb_set_red(other);
				node = parent;
				parent = rb_parent(node);
			}
			else
			{
				if (!other->rb_left || rb_is_black(other->rb_left))
				{
					rb_set_black(other->rb_right);
					rb_set_red(other);
					__rb_rotate_left(other, root);
					other = parent->rb_left;
				}
				rb_set_color(other, rb_color(parent));
				rb_set_black(parent);
				rb_set_black(other->rb_left);
				__rb_rotate_right(parent, root);
				node = root->rb_node;
				break;
			}
		}
	}
	if (node)
		rb_set_black(node);
}

void rb_erase(struct RBNode *node, RBTreePtr root)
{
	struct RBNode *child, *parent;
	int color;

	if (!node->rb_left)
		child = node->rb_right;
	else if (!node->rb_right)
		child = node->rb_left;
	else
	{
		struct RBNode *old = node, *left;

		node = node->rb_right;
		while ((left = node->rb_left) != NULL)
			node = left;

		if (rb_parent(old)) {
			if (rb_parent(old)->rb_left == old)
				rb_parent(old)->rb_left = node;
			else
				rb_parent(old)->rb_right = node;
		} else
			root->rb_node = node;

		child = node->rb_right;
		parent = rb_parent(node);
		color = rb_color(node);

		if (parent == old) {
			parent = node;
		} else {
			if (child)
				rb_set_parent(child, parent);
			parent->rb_left = child;

			node->rb_right = old->rb_right;
			rb_set_parent(old->rb_right, node);
		}

		node->rb_parent_color = old->rb_parent_color;
		node->rb_left = old->rb_left;
		rb_set_parent(old->rb_left, node);

		goto color;
	}

	parent = rb_parent(node);
	color = rb_color(node);

	if (child)
		rb_set_parent(child, parent);
	if (parent)
	{
		if (parent->rb_left == node)
			parent->rb_left = child;
		else
			parent->rb_right = child;
	}
	else
		root->rb_node = child;

 color:
	if (color == RB_BLACK)
		__rb_erase_color(child, parent, root);
}

static void rb_augment_path(struct RBNode *node, rb_augment_f func, void *data)
{
	struct RBNode *parent;

up:
	func(node, data);
	parent = rb_parent(node);
	if (!parent)
		return;

	if (node == parent->rb_left && parent->rb_right)
		func(parent->rb_right, data);
	else if (parent->rb_left)
		func(parent->rb_left, data);

	node = parent;
	goto up;
}

/*
 * after inserting @node into the tree, update the tree to account for
 * both the new entry and any damage done by rebalance
 */
void rb_augment_insert(struct RBNode *node, rb_augment_f func, void *data)
{
	if (node->rb_left)
		node = node->rb_left;
	else if (node->rb_right)
		node = node->rb_right;

	rb_augment_path(node, func, data);
}

struct RBNode *rb_next(const struct RBNode *node)
{
	struct RBNode *parent;

	if (rb_parent(node) == node)
		return NULL;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	if (node->rb_right) {
		node = node->rb_right; 
		while (node->rb_left)
			node=node->rb_left;
		return (struct RBNode *)node;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	while ((parent = rb_parent(node)) && node == parent->rb_right)
		node = parent;

	return parent;
}

/*
 * before removing the node, find the deepest node on the rebalance path
 * that will still be there after @node gets removed
 */
struct RBNode *rb_augment_erase_begin(struct RBNode *node)
{
	struct RBNode *deepest;

	if (!node->rb_right && !node->rb_left)
		deepest = rb_parent(node);
	else if (!node->rb_right)
		deepest = node->rb_left;
	else if (!node->rb_left)
		deepest = node->rb_right;
	else {
		deepest = rb_next(node);
		if (deepest->rb_right)
			deepest = deepest->rb_right;
		else if (rb_parent(deepest) != node)
			deepest = rb_parent(deepest);
	}

	return deepest;
}

/*
 * after removal, update the tree to account for the removed entry
 * and any rebalance damage.
 */
void rb_augment_erase_end(struct RBNode *node, rb_augment_f func, void *data)
{
	if (node)
		rb_augment_path(node, func, data);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
struct RBNode *rb_first(const RBTreePtr root)
{
	struct RBNode	*n;

	n = root->rb_node;
	if (!n)
		return NULL;
	while (n->rb_left)
		n = n->rb_left;
	return n;
}

struct RBNode *rb_last(const RBTreePtr root)
{
	struct RBNode	*n;

	n = root->rb_node;
	if (!n)
		return NULL;
	while (n->rb_right)
		n = n->rb_right;
	return n;
}


struct RBNode *rb_prev(const struct RBNode *node)
{
	struct RBNode *parent;

	if (rb_parent(node) == node)
		return NULL;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	if (node->rb_left) {
		node = node->rb_left; 
		while (node->rb_right)
			node=node->rb_right;
		return (struct RBNode *)node;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	while ((parent = rb_parent(node)) && node == parent->rb_left)
		node = parent;

	return parent;
}

void rb_replace_node(struct RBNode *victim, struct RBNode *new,
		     RBTreePtr root)
{
	struct RBNode *parent = rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	if (parent) {
		if (victim == parent->rb_left)
			parent->rb_left = new;
		else
			parent->rb_right = new;
	} else {
		root->rb_node = new;
	}
	if (victim->rb_left)
		rb_set_parent(victim->rb_left, new);
	if (victim->rb_right)
		rb_set_parent(victim->rb_right, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}

RBTree
makeRBTree(void *(*getKey)(struct RBNode *), int (*cmpKey)(void *, void *),
        struct RBNode *(*makeNode)(void *key),
        void    (*freeNode)(struct RBNode *))
{
        return (RBTree){ getKey, cmpKey, makeNode, freeNode, NULL };
}

static void
destroyTree(struct RBNode *tree, void (*freeFunc)(struct RBNode *))
{
        if(tree == NULL)
                return;
        if(tree->rb_left != NULL)
                destroyTree(tree->rb_left, freeFunc);
        if(tree->rb_right != NULL)
                destroyTree(tree->rb_right, freeFunc);
        freeFunc(tree);
}

void
destroyRBTree(RBTree tree)
{
        if(tree.freeNode)
                destroyTree(tree.rb_node, tree.freeNode);
}

struct RBNode *
rbGetRoot(RBTree tree)
{
        return tree.rb_node;
}

bool
rbInsert(RBTreePtr tree, void *key)
{
	struct RBNode **new = &tree->rb_node, *parent = NULL;
	while(*new) {
		struct RBNode *this = *new;
		int result = tree->cmpKey(key, tree->getKey(this));
		parent = *new;
		if(result < 0) {
			new = &((*new)->rb_left);
		} else if(result > 0) {
			new = &((*new)->rb_right);
		} else {
			return false;
		}
	}
	struct RBNode *node = tree->makeNode(key);
	rb_link_node(node, parent, new);
	rb_insert_color(node, tree);
	return true;
}

struct RBNode *
rbSearch(RBTree root, void *key)
{
	struct RBNode *node = root.rb_node;
	while(node) {
		struct RBNode *data = node;
		int result = root.cmpKey(key, root.getKey(data));
		if(result < 0) {
			node = node->rb_left;
		} else if(result > 0) {
			node = node->rb_right;
		} else {
			return data;
		}
	}
	return NULL;
}

void
rbDelete(RBTreePtr tree, void *key)
{
	struct RBNode *node = rbSearch(*tree, key);
	if(node) {
		rb_erase(node, tree);
		if(tree->freeNode)
			tree->freeNode(node);
	}
}
