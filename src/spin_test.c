#include "spin.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

static spin_lock_t lock = INIT_SPIN_LOCK;

void * 
test_func1(void* ptr)
{
	while(1) {
		printf("func1 try in...\n");
		spin_lock(&lock);
		printf("func1 in\n");
		sleep(2);
		printf("func1 out\n");
		spin_unlock(&lock);
		sleep(3);
	}
	return NULL;
}

void *
test_func2(void* ptr)
{
	while(1) {
		printf("func2 try in...\n");
		spin_lock(&lock);
		printf("func2 in\n");
		sleep(1);
		printf("func2 out\n");
		spin_unlock(&lock);
		sleep(3);
	}
    	return NULL;
}

void *
test_func3(void* ptr)
{
	while(1) {
		printf("func3 try lock...\n");
		if(spin_try_lock(&lock)) {
			printf("lock success...\n");
			sleep(1);
			printf("func3 out\n");
			spin_unlock(&lock);
		} else {
			printf("lock false...\n");
		}
	}
	return NULL;
}

int 
main(int argc, char *argv[])
{
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, test_func1, NULL);
    pthread_create(&tid2, NULL, test_func2, NULL);
    pthread_create(&tid3, NULL, test_func3, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    return 0;
}
