#include "fsm.h"
#include "mem.h"
#include "rb_tree.h"
#include "container.h"

struct EventLink {
	int event;
	int dst_state;
};

typedef void (*Hander)(int, int, int);

//need by makeRBTree()
static void *
toGetKey(struct RBNode *ptr)
{
	return &container_of(ptr, struct FSMNode, rb_node)->state;
}

//need by makeRBTree()
static int
toCmpKey(void *p1, void *p2)
{
	return *(int *)p1 - *(int *)p2;
}

//need by makeRBTree()
static struct RBNode *
toMakeNode(void *key)
{
	struct FSMNode *node = malloc(sizeof(struct FSMNode));
	node->state = *(int *)key;
	node->events = makeMem(sizeof(struct EventLink));
	node->trans_hander = NULL;
	return &node->rb_node;
}
	
//need by makeRBTree()
static void
toFreeNode(struct RBNode *node)
{
	destroyMem(container_of(node, struct FSMNode, rb_node)->events);
	free(container_of(node, struct FSMNode, rb_node));
}

fsm_t
makeFSMType(void (*err_hander)(int, int))
{
	return (fsm_t){ .err_hander = err_hander, 
			.rb_tree = makeRBTree(toGetKey, toCmpKey, toMakeNode, toFreeNode),
			.cur_state = NULL };
}

void 
fsmLink(fsm_t_ptr fsm_p, int from_state, int event, int to_state)
{
	//确保from_state和to_state在红黑树中
	rbInsert(&fsm_p->rb_tree, &from_state);
	rbInsert(&fsm_p->rb_tree, &to_state);
	struct FSMNode *node = container_of(rbSearch(fsm_p->rb_tree, &from_state), struct FSMNode, rb_node);
	struct EventLink link = { event, to_state };
	SET_TYPE_MEM(&node->events, struct EventLink, getMemIndex(node->events) / sizeof(struct EventLink), link);
}

void
fsmSetCurState(fsm_t_ptr fsm_p, int state)
{
	//确保state和在红黑树中
	rbInsert(&fsm_p->rb_tree, &state);
	fsm_p->cur_state = container_of(rbSearch(fsm_p->rb_tree, &state), struct FSMNode, rb_node);
}

int
fsmGetCurState(fsm_t fsm)
{
	return fsm.cur_state->state;
}

void
fsmSetHander(fsm_t_ptr fsm_p, int state, void (*hander)(int, int, int))
{
	//确保state和在红黑树中
	rbInsert(&fsm_p->rb_tree, &state);
	struct FSMNode *node = container_of(rbSearch(fsm_p->rb_tree, &state), struct FSMNode, rb_node);
	node->trans_hander = hander;
}

void 
fsmPushEvent(fsm_t_ptr fsm_p, int event)
{
	int pre_state = fsm_p->cur_state->state;
	for(int i = 0; i < getMemIndex(fsm_p->cur_state->events) / sizeof(struct EventLink); i++) {
		if(GET_TYPE_MEM(&fsm_p->cur_state->events, struct EventLink, i)->event == event) {
			fsmSetCurState(fsm_p, GET_TYPE_MEM(&fsm_p->cur_state->events, struct EventLink, i)->dst_state);
			if(fsm_p->cur_state->trans_hander != NULL)
				fsm_p->cur_state->trans_hander(pre_state, event, fsm_p->cur_state->state);
			return;
		}
	}
	if(fsm_p->err_hander != NULL)
		fsm_p->err_hander(fsm_p->cur_state->state, event);
}

void
destroyFSMType(fsm_t fsm)
{
	destroyRBTree(fsm.rb_tree);
}
