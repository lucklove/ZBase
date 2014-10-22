#pragma once

#include "mem.h"
#include "rb_tree.h"

struct FSMNode {
	int state;
	mem_t events;
	void (*trans_hander)(int, int, int);
	struct RBNode rb_node;
};

typedef struct {
	void (*err_hander)(int, int);
	struct FSMNode *cur_state;
	RBTree rb_tree;
} fsm_t, *fsm_t_ptr;

fsm_t makeFSMType(void (*)(int, int));
void fsmSetCurState(fsm_t_ptr, int);
int  fsmGetCurState(fsm_t);
void fsmSetHander(fsm_t_ptr, int, void (*)(int, int, int));
void fsmLink(fsm_t_ptr, int, int, int);
void fsmPushEvent(fsm_t_ptr, int);
void destroyFSMType(fsm_t);
