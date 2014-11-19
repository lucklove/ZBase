/**************************************************************************
 * zobject.h                                                              * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>			  *
 **************************************************************************/

#pragma once

#include "mem.h"
#include "ref.h"
#include "exception.h"
#include "rb_tree.h"
#include <stdbool.h>
#include <stdlib.h>

/** class information. */
struct ZObjClass {
	const char *class_name;
	struct ZObjClass *parent;
	void *(*constructor)(void *, void *);
	void (*destructor)(void *);
	unsigned int instance_size; 	/**< alloc memory for instance. */
	mem_t interfaces;
	mem_t class_body;
	struct RBNode rb_node;	    	/**< to store this struct in red black tree. */
};

struct ZObjInstance {
	struct ZObjClass *class;	/**< class which this instance belong to. */
	struct ZObjInstance *parent;	/**< parent instance. */
	void *instance_body;		/**< user's struct body. */
	ref_t ref_node;			/**< refrence count. */
};

struct ZObjInterface {
	const char *interface_name;
	void *interface_body;		/**< user's instance body. */
};

/**
 * \brief Init object system
 * \param thread_identifer 
 * 	The fuction is given to exception system, so that the system can 
 * 	identify diffrent thread. This if for thread safe, if you use 
 * 	single thread or will not use exception, then you can pass NULL.
 * \note This should be called before any other opreation of object system
 */
void zObjInit(unsigned long (*thread_identifier)(void));

/**
 * \brief Find prarent(or self)'s class body
 * \param self_name What class's parent you want to find?
 * \param parent_name 
 * 	  What parent you want to find? the self's parent?
 * 	  or the parent's parent?
 * \return The class body of parent(which is defined by user).
 * \example 
 * 	void *class_body = zGetClassByName("dog", "anamal"); -- to 
 * 	find dog's anamal parent.
 * \example void *class_body = zGetClassByName("dog", NULL); -- to find dog self.
 */
void *zGetClassByName(const char *self_name, const char *parent_name);

/**
 * \brief Find class body or parent class body for an instance.
 * \param instance From the given instance, we can get it's class and parent class.
 * \param class_name The same with zGetClassByName.
 * \return The class body of parent(which is defined by user).
 * \example void *class = zGetClassByIntance(ins, "base_class");
 * \example void *class = zGetClassByInstance(ins, NULL). -- return ins's class's class body.
 */
void *zGetClassByInstance(struct ZObjInstance *instance, const char *class_name);

/**
 * \brief Find instance body or parent instance body for an instance.
 * \param instance Find the given instance's body or it's parent's body.
 * \param instance_name 
 * 	  To identify which instance body should be return(the instance 
 * 	  self or one of it's parent.
 * \return The instance body(which is defined by user).
 */
void *zGetInstance(struct ZObjInstance *instance, const char *instance_name);

/**
 * \brief Get interface body of a class.
 * \param class_name The dest class.
 * \param interface_name Which interface of the dest class you want.
 * \return The dest class's dest interface body.
 */
void *zGetInterfaceByName(const char *class_name, const char *interface_name);

/**
 * \brief Get interface body of an instance.
 * \param instance Find the given instance's dest interface.
 * \param interface The dest interface name.
 * \return The dest interface body.
 */
void *zGetInterfaceByInstance(struct ZObjInstance *instance, const char *interface);

/**
 * \brief Bind an interface for a class.
 * \param class_name The dest class you want to bind interface to.
 * \param interface_name The dest interface you want to bind to class.
 * \example zAddInterface("my first class", "my first interface");
 */
bool zAddInterface(const char *class_name, const char *interface_name);

/**
 * \brief Regest an interface to object system, so we can use it by it's
 * name(which is const char *).
 * \param interface_name The name you want to regist.
 * \param body_size 
 * 	  memory buffer size reserved for interface body defined by user
 * \return If success true, else false.
 * \example zRegistInterface("interface", sizeofo(struct MyInterface));
 */
bool zRegistInterface(const char *interface_name, unsigned int body_size);

/**
 * \brief Add a parent for an interface.
 * \param self_name The dest interface which has just been regested.
 * \param parent_name The parent interface name.
 * \return if success, true, else, false.
 */
bool zInterfaceAddParent(const char *self_name, const char *parent_name);

/**
 * \brief New an instance(alloc memory and init it).
 * \param class_name The class which you want to instance.
 * \param data init data which pass to the constructor of dest class.
 * \return new instance refrence.
 */
struct ZObjInstance *zNewInstance(const char *class_name, void *data);

/**
 * \brief Increase an instance's refrence counter so that you can 
 * 	pass it to other thread.
 * \param instance The instance you want to pass.
 */
void zObjIncRef(struct ZObjInstance *instance);

/**
 * \brief 
 * 	Decrease an instance's refrence counter, if the counter reach 0, 
 * 	call user's destructor and free resource of this instance.
 * \param instance The instance you want to throw away.
 */
void zObjDecRef(struct ZObjInstance *instance);

/**
 * \brief 
 * 	Decrease an instance's refrence counter, if the counter reach 0, 
 * 	call user's destructor and free resource of this instance.
 * \param instance The instance you want to throw away.
 */
void zDesInstance(struct ZObjInstance *instance);

/**
 * \brief Regist a class to object system.
 * \param class_name The class's name.
 * \param parent_name Inheritant from which class, if none, NULL instead.
 * \param constructor User function which will be called when construct an instance.
 * \param destructor User function whicth will be called when destruct an instance.
 * \param ins_size Instance size, it's usefull when alloc memory for an instance.
 * \param class_body A struct instance, contain class method.
 * \param class_size Size of class_body.
 * \return if success, true, else, false.
 * \example 
 * 	zRegistClass("my_first_class", NULL, cons, des, sizeof(struct MyInstance),
 * 		&class_body, sizeof(struct MyClass));
 */
int zRegistClass(const char *class_name, const char *parent_name, 
		void *(*constructor)(void *, void *), void (*destructor)(void *), 
		unsigned int ins_size, void *class_body, unsigned int class_size);

/**
 * \brief 
 * 	Get dest class body from an instance, the instance's class must be in the
 * 	inheritant chain of dest class.
 * \param ins The dest instance.
 * \param name Dest class name.
 * \param type Dest class body type.
 * \example Z_OBJ_TO_CLASS(instance, "base_class", struct BaseClass)->func();
 */
#define Z_OBJ_TO_CLASS(ins, name, type) ((type *)zGetClassByInstance(ins, name))

/**
 * \brief 
 * 	Get an interface body from an instance, dest interface must be added in the
 * 	instance's class.
 * \param ins The dest instance.
 * \param interface_name Dest interface name.
 * \param type Dest instance body type.
 * \example Z_OBJ_TO_INTERFACE(instance, "my_interface", struct MyInterface)->func();
 */
#define Z_OBJ_TO_INTERFACE(ins, interface_name, type) ((type *)zGetInterfaceByInstance(ins, interface_name))

/**
 * \brief 
 * 	Get an instance body from an instance, dest instance must be in the inheritant
 * 	chain of instance.
 * \param ins The instance.
 * \class_name Which instance body you want, if it's NULL, the ins's body will be return.
 * \type Dest instance body type.
 * \example Z_OBJ_TO_INSTANCE(instance, NULL, struct MyInstance)->msg = "hello";
 */
#define Z_OBJ_TO_INSTANCE(ins, class_name, type) ((type *)zGetInstance(ins, class_name))

/**
 * \brief Get an interface body of given class to implement.
 * \param class_name Dest class_name.
 * \param interface_name Dest instance_name, the instance should be added in dest class.
 * \param type Type of dest interface body.
 * \example Z_IMP_INTERFACE("my_first_class", "smile", struct Smile)->smile = haha;
 */
#define Z_IMP_INTERFACE(class_name, interface_name, type) ((type *)zGetInterfaceByName(class_name, interface_name))

/**
 * \brief Get parent class body from a class.
 * \param self Self class name.
 * \param name Parent class name.
 * \param type Parent class body type.
 * \example Z_CLASS_TO_CLASS("fish", "anamal", struct Anamal)->sleep = sleep;
 */
#define Z_CLASS_TO_CLASS(self, name, type) ((type *)zGetClassByName(self, name))
