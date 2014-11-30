/**************************************************************************
 * fsm.h                                                                  * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 * implemented in fsm.c                                                   *
 **************************************************************************/

#pragma once

#include "mem.h"
#include "rb_tree.h"

#ifdef __cplusplus
extern "C" {
#endif

/** state node. */
struct FSMNode {
	int state;				/**< state of this node. */
	mem_t events;				/**< accept event and dest state. */
	void (*trans_hander)(int, int, int);	/**< called when translate to this node. */
	struct RBNode rb_node;			/**< to store this node in red black tree. */
};

typedef struct {
	/** called when translate accept an invalid event. */
	void (*err_hander)(int, int);
	/** current state node. */
	struct FSMNode *cur_state;
	/** the red black tree which store all state node. */
	RBTree rb_tree;
} fsm_t, *fsm_t_ptr;

/**
 * \brief Generate a fsm handle(fsm_t).
 * \param err_hander
 * 	Function to handle error when the event is invalid for
 * 	current state. 
 * 	The first param is current state and the second param
 * 	is the invailid event number.
 * \return A new handel(fsm_t).
 */ 	
fsm_t makeFSMType(void (*err_hander)(int, int));

/**
 * \brief Set current state of fsm.
 * \param fsm_p Pointer point to dest fsm.
 * \param state Dest state number.
 */ 
void fsmSetCurState(fsm_t_ptr fsm_p, int state);

/**
 * \brief Get current state.
 * \param fsm Dest fsm.
 * \return Current state number.
 */ 
int  fsmGetCurState(fsm_t fsm);

/**
 * \brief Set a hander for dest state.
 * \param fsm_p Pointer point to dest fsm.
 * \param state Dest state.
 * \param hander
 * 	Hander set for dest state.
 * 	The first param of hander is previous state.
 * 	The second param of hander is the current event.
 * 	The third param of hander is current state.
 */ 	
void fsmSetHander(fsm_t_ptr fsm_p, int state, void (*hander)(int, int, int));

/**
 * \brief 
 * 	Link two state so that we can translate from the first state
 * 	to the second by the event.
 * \param fsm_p Pointer point to dest fsm.
 * \param from_state Link start from this state.
 * \param event Param from_state can accept this event.
 * \param to_state 
 * 	When param from_state accept dest evnet, the current 
 * 	state change to this state.
 */ 	
void fsmLink(fsm_t_ptr fsm_p, int from_state, int event, int to_state);

/**
 * \brief Push an event to dest fsm.
 * \param Pointer point to dest fsm.
 * \param event Event number.
 */ 
void fsmPushEvent(fsm_t_ptr fsm_p, int event);

/**
 * \brief Release dest fsm's resource.
 * \param fsm_p Pointer point to dest fsm.
 */ 
void destroyFSMType(fsm_t fsm_p);

#ifdef __cplusplus
}
#endif
