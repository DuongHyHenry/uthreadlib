#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <uthread.h>

int global_var = 0;

static void thread2(void* arg)
{
	(void)arg;
	printf("thread2 - suppose to be thread2\n");
	global_var = 1;
	printf("global_var - %d\n", global_var);
}

static void thread1(void* arg)
{
	(void)arg;
	uthread_create(thread2, NULL);
	printf("thread1 - suppose to be thread1\n");
	printf("global_var - %d\n", global_var);
}

int main(void)
{
	uthread_run(true, thread1, NULL);
	return 0;
}
