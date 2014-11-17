#pragma once

#include "zobject.h"
#include <setjmp.h>
#include <stdbool.h>

#define throw(instance)	__throw(instance)

#define rethrow() 						\
do {								\
	__pop_jmp_point();					\
	__throw(__get_cur_error());				\
} while(0)

#define try if(!setjmp(__push_jmp_point()))

#define catch(instance, class_name)				\
    else if(zGetInstance(__get_cur_error(), class_name)) {	\
	struct ZObjInstance *instance = __get_cur_error();
	
#define otherwise } else {

#define finally } __pop_jmp_point();

//bool __push_jmp_point(void);
void * __push_jmp_point(void);
void __pop_jmp_point(void);
struct ZObjInstance *__get_cur_error(void);
void __throw(struct ZObjInstance *);
void exceptionInit(unsigned long (*)(void));
