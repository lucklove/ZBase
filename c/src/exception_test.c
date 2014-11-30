#include "spin.h"
#include "zobject.h"
#include <stdio.h>
#include <pthread.h>

struct ErrorClass {
	void (*deal_error)(struct ZObjInstance *, const char *);
};

struct ErrorIns {
	char *error_name;
};

void *
cons(void *ins, void *data)
{
	((struct ErrorIns *)ins)->error_name = data;
	return NULL;
}

void
des(void *ins)
{
	printf("base error destructor called\n");
}

void
handle(struct ZObjInstance *err, const char *msg)
{
	printf("error occured: %s, %s\n", Z_OBJ_TO_INSTANCE(err, "base_error", struct ErrorIns)->error_name, msg);
}

void *
func2()
{
	while(true) {
		try {
			struct ZObjInstance *err = zNewInstance("base_error", "error in func2");
			throw(err);
		} catch(ins, "base_error") {
			Z_OBJ_TO_CLASS(ins, "base_error", struct ErrorClass)->deal_error(ins, "this is func2");
			zDesInstance(ins);
		} finally {
			printf("func2 error finnaly\n");
		};
	}
	return NULL;
}

void *
func1()
{
	while(true) {
		try {
			throw(zNewInstance("base_error", "error in func1"));
		} catch(ins, "base_error") {
			Z_OBJ_TO_CLASS(ins, "base_error", struct ErrorClass)->deal_error(ins, "this is func1");
			zDesInstance(ins);
		} finally {
			printf("func1 error finnaly\n");
		};
	}
	return NULL;
}
	
int
main(int argc, char *argv[])
{
	zObjInit(pthread_self);
	pthread_t tid1, tid2;
	struct ErrorClass error_class = { handle };
	zRegistClass("base_error", NULL, cons, des, sizeof(struct ErrorIns), &error_class, sizeof(error_class));
	pthread_create(&tid1, NULL, func1, NULL);
	pthread_create(&tid2, NULL, func2, NULL);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	return 0;
}
