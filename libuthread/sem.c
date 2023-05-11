#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	/* TODO Phase 3 */
	queue_t waitingQueue;
	int count;
};

sem_t sem_create(size_t count)
{
	/* TODO Phase 3 */
	sem_t newSem = calloc(1, sizeof(sem_t));
	newSem->count = (int)count;
	newSem->waitingQueue = queue_create();
	//printf("Semaphore Created\n");
	return newSem;
}

int sem_destroy(sem_t sem)
{
	/* TODO Phase 3 */
	free(sem->waitingQueue);
	free(sem);
	//printf("Semaphore Destroyed\n");
	return 0;
}

int sem_down(sem_t sem)
{
	/* TODO Phase 3 */
	// printf("Sem #%d Down Count: %d\n", sem->semNumber, sem->count);
	if (sem->count == 0) {
		uthread_block();
		queue_enqueue(sem->waitingQueue, uthread_current());
		uthread_yield();
		return -1;
	}
	//printf("Resource Available\n");
	sem->count--;
	return 0;
}

int sem_up(sem_t sem)
{
	/* TODO Phase 3 */

	sem->count++;
	void *tempQueue;
	queue_dequeue(sem->waitingQueue, &tempQueue);
	// printf("%d\n", queue_length(sem->waitingQueue));
	uthread_unblock(uthread_current());
	uthread_yield();
	// printf("Thread In Sem #%d Unblocked\n", sem->semNumber);
	// printf("Sem #%d Up Count: %d\n", sem->semNumber, sem->count);
	return 0;
}

