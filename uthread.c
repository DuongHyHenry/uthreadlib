#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

//Global Variables
queue_t READY;
queue_t BLOCKED;

struct uthread_tcb {
	uthread_ctx_t *context;
	void *stackPointer;
};

typedef struct uthread_tcb* uthread_tcb_t;

uthread_tcb_t RUNNING;
uthread_tcb_t IDLE;

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2/3 */
	return RUNNING;
}

void uthread_yield(void)
{
	// /* TODO Phase 2 */ 
	uthread_tcb_t nextThread;
	uthread_tcb_t oldThread = RUNNING;
	queue_enqueue(READY, RUNNING);
    queue_dequeue(READY, (void*)&nextThread);
	RUNNING = nextThread;
	uthread_ctx_switch(oldThread->context, nextThread->context);
}

void uthread_exit(void)
{
	/* TODO Phase 2 */
    uthread_tcb_t exitThread;
	queue_dequeue(READY, (void*)&exitThread);
	uthread_tcb_t oldThread;
	oldThread = RUNNING;
	uthread_ctx_switch(oldThread->context, exitThread->context);
	free(exitThread->context);
	uthread_ctx_destroy_stack(exitThread->stackPointer);
	free(exitThread);
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	// creates thread
	uthread_tcb_t newThread = calloc(1, sizeof(uthread_tcb_t));
	newThread->stackPointer = uthread_ctx_alloc_stack();
	newThread->context = calloc(1, sizeof(uthread_ctx_t));
	// sets up queues for scheduling
	uthread_ctx_init(newThread->context, newThread->stackPointer, func, arg);
	queue_enqueue(READY, newThread);
	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
    if (preempt == true) {
        printf("bruh\n");
    }
	READY = queue_create();
    BLOCKED = queue_create();

    uthread_tcb_t idleThread;

    uthread_create(NULL, NULL);
    queue_dequeue(READY, (void*)&idleThread);
    IDLE = idleThread;
    RUNNING = idleThread;
    uthread_create(func, arg);
    while (1) {
        if (queue_length(READY) == 0) {
            uthread_exit();
            break;
        }
        else {
            uthread_yield();
        }
    }
    return 0;
}

void uthread_block(void)
{
	/* TODO Phase 3 */
	queue_enqueue(BLOCKED, RUNNING);
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 3 */
	queue_dequeue(BLOCKED, (void*)&uthread);
	queue_enqueue(READY, uthread);
}