#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "queue.h"
#include "uthread.h"

//Global Variables
queue_t READYQUEUE;
queue_t BLOCKEDQUEUE;

//defines uthread struct with a context pointer and a stack pointer
struct uthread_tcb {
	uthread_ctx_t *context;
	void *stackPointer;
};

//pointer struct to uthread struct
typedef struct uthread_tcb* uthread_tcb_t;

//defines the CURRENTLYRUNNING and CURRENTLYIDLE global threads
uthread_tcb_t CURRENTLYRUNNING;
uthread_tcb_t CURRENTLYIDLE;

//returns the currently running thread
struct uthread_tcb *uthread_current(void)
{
	
	//returns the thread currently running
	return CURRENTLYRUNNING;
}

//yield function
void uthread_yield(void)
{

	//if the ready queue is empty, return
	if (queue_length(READYQUEUE) == 0){
		return;
	}

	//calls a new thread to dequeue into
	uthread_tcb_t nextThread;

	//puts currently running thread into oldThread as a temp variable
	uthread_tcb_t oldThread = CURRENTLYRUNNING;

	//enqueues running thread to the end of the ready queue
	queue_enqueue(READYQUEUE, CURRENTLYRUNNING);

	//decrements the queue and puts the first thread into nextThread
    queue_dequeue(READYQUEUE, (void*)&nextThread);

	//defines nextThread as the currently runnning thread
	CURRENTLYRUNNING = nextThread;

	//switches the contexts of the old running thread and the new one
	uthread_ctx_switch(oldThread->context, nextThread->context);
}

void uthread_exit(void)
{

	//defines a thread to dequeue the queue into
    uthread_tcb_t exitThread;

	//dequeues a thread that's going to exit
	queue_dequeue(READYQUEUE, (void*)&exitThread);

	//puts currently running thread into oldThread as a temp variable
	uthread_tcb_t oldThread = CURRENTLYRUNNING;
	
	//switches the contexts of the old running thread and the new one
	uthread_ctx_switch(oldThread->context, exitThread->context);

	//frees the dynamically allocated space of the thread
	free(exitThread->context);
	uthread_ctx_destroy_stack(exitThread->stackPointer);
	free(exitThread);
}

int uthread_create(uthread_func_t func, void *arg)
{

	//dynamically allocates space for all the threads
	uthread_tcb_t newThread = calloc(1, sizeof(uthread_tcb_t));
	newThread->stackPointer = uthread_ctx_alloc_stack();
	newThread->context = calloc(1, sizeof(uthread_ctx_t));

	//if memory failed to allocate, return -1
	if ((newThread == NULL) || (newThread->stackPointer == NULL) || (newThead->context == NULL)) {
		return -1;
	}

	//initializes the context of the thread
	uthread_ctx_init(newThread->context, newThread->stackPointer, func, arg);

	//if context failed to create, return -1
	if (newThread->context == NULL) {
		return -1;
	}

	//enqueues thread into ready queue for future use
	queue_enqueue(READYQUEUE, newThread);
	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{

	//we were not able to finish preempt in time
    (void)preempt;

	//creates both ready and blocked queues
	READYQUEUE = queue_create();
    BLOCKEDQUEUE = queue_create();

	//if queues failed to allocate, return -1
	if ((READYQUEUE == NULL) || (BLOCKEDQUEUE == NULL)) {
		return -1;
	}

	//defines the idle thread
    uthread_tcb_t idleThread;

	//creates an idle thread with no func or arg
    uthread_create(NULL, NULL);

	//dequeues the just created idle thread
    queue_dequeue(READYQUEUE, (void*)&idleThread);

	//sets both idle and running to the idle thread
    CURRENTLYIDLE = idleThread;
    CURRENTLYRUNNING = idleThread;

	//creates the initial thread with func and arg
    uthread_create(func, arg);

	//infinite while loop
    while (1) {
		
		//breaks while loop if there are no more threads left
        if (queue_length(READYQUEUE) == 0) {
			
			//calls the exit function to initialize exiting of a thread
            uthread_exit();

			//breaks out of the loop
            break;
        }
        else {

			//otherwise, the idle thread yields if there are still threads
            uthread_yield();
        }
	}
    return 0;
}

void uthread_block(void)
{
	
	//enqueues the currently running thread into the blocked queue
	queue_enqueue(BLOCKEDQUEUE, CURRENTLYRUNNING);
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	
	//dequeues the first thread from blocked and puts it into uthread
	queue_dequeue(BLOCKEDQUEUE, (void*)&uthread);
}