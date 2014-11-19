/**************************************************************************
 * exception.h                                                            * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 **************************************************************************/

#pragma once

#include "zobject.h"
#include <setjmp.h>
#include <stdbool.h>

/**
 * \brief throw an exception.
 * \param instance exception instance.
 * \example throw(zNewInstance("base_exception", "some bad thing happen");
 */
#define throw(instance)	__throw(instance)

/** rethrow an exception. */
#define rethrow() 						\
do {								\
	__pop_jmp_point();					\
	__throw(__get_cur_exception());				\
} while(0)

/**
 * \brief try block.
 * \example
 * 	try {
 * 		throw(zNewInstance("base_exception", "error happened"));
 * 	} catch(ins, "base_exception) {
 * 		do_something(ins);
 * 	} otherwise {
 * 		do_anotherthing();
 * 		rethrow();
 * 	} finally {
 * 		finnaly_thing();
 * 	}
 */
#define try if(!setjmp(__push_jmp_point()))

#define catch(instance, class_name)				\
    else if(zGetInstance(__get_cur_exception(), class_name)) {	\
	struct ZObjInstance *instance = __get_cur_exception();
	
#define otherwise } else {

#define finally } __pop_jmp_point();

/**
 * \note the fellow functions, include __push_jmp_point, __pop_jmp_point,
 * 	 __get_cur_exception, __throw, are used by micro, and they should 
 * 	 never be used directly by user!!!
 */
void * __push_jmp_point(void);
void __pop_jmp_point(void);
struct ZObjInstance *__get_cur_exception(void);
void __throw(struct ZObjInstance *);
