#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

//defines semaphore struct with a queue for waiting threads and a count
struct semaphore {
	queue_t waitingQueue;
	int count;
};

//creates a semaphore with initial count
sem_t sem_create(size_t count)
{

	//dynamically allocates space for the semaphore and returns
	sem_t newSem = calloc(1, sizeof(sem_t));
	newSem->count = (int)count;
	newSem->waitingQueue = queue_create();

	//if newSem or its queue fails to allocate, return NULL;
	if ((newSem == NULL) || (newSem->waitingQueue == NULL)){
		return NULL;
	}
	return newSem;
}

int sem_destroy(sem_t sem)
{
	
	//if sem is NULL, return -1
	if (sem == NULL) {
		return -1;
	}

	//frees the dynamically allocated space of the semaphore
	free(sem->waitingQueue);
	free(sem);
	return 0;
}

int sem_down(sem_t sem)
{

	//if sem is NULL, return -1
	if (sem == NULL) {
		return -1;
	}

	//if count is 0, the current thread becomes blocked and is put in waiting and returns
	if (sem->count == 0) {

		//blocks the current thread
		uthread_block();

		//puts it into waiting for the semaphore
		queue_enqueue(sem->waitingQueue, uthread_current());

		//yields to the next queue
		uthread_yield();
		return 0;
	}

	//decrements semaphore count if resource was taken
	sem->count--;
	return 0;
}

int sem_up(sem_t sem)
{

	//if sem is NULL, return -1
	if (sem == NULL) {
		return -1;
	}

	//increments resource count
	sem->count++;

	//defines a new thread to put the dequeued element into
	void *newThread;

	//dequeues the thread waiting the longest and puts it in newThread
	queue_dequeue(sem->waitingQueue, &newThread);

	//unblocks the currently running thread
	uthread_unblock(uthread_current());

	//yields to the next thread
	uthread_yield();
	return 0;
}

