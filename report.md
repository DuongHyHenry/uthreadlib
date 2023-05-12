# ECS150 Project 2: User-level thread library

Andy Truong, antruon@ucdavis.edu, 918297332
Henry Duong, henduong@ucdavis.edu, 919804643

## Introduction

The main goal of this project is to use the given API to implement a thread 
library with its own scheduling process, use of semaphores to handle multiple 
threads accessing shared resources, and using preemption to handle specific 
scenarios. Currently, our programs are able to handle the first three tasks up 
until preemption(phase 4)From this project, we learned more about thread 
management and how the Operating System handles concurrently running multiple 
threads and when/how to schedule them. In addition to this, this project gave us 
a greater understanding of Makefiles as we had to use the Makefile to generate 
a static library and be able to use multiple C files with its associated API 
files.

## Implementation

### Phase 1: Queue API

For this phase, we had to implement a FIFO queue(first in first out). We chose 
to create a doubly linked list for our underlying data structure, since it would 
allow us to easily retreive the data at the top of the queue. Linking the 
individual nodes would allow us to perform the given operations(such as 
`queue_create()` and `queue_enqueue()`) in constant time, which was specified by 
the rubric. Initially we attempted to use an array as the underlying data 
structure, however it was hard to allocate space and manipulate it in constant 
time, therefore we had to rethink and redo our implementation.

We created a struct for the items within the queue and for the queue itself. 
*Items* has member variables *prev*, *next*, and *data*. For the queue struct, 
we had a *head* and *tail* of type *item*, and a counter variable to keep track 
of the number of items. The *prev* and *next* variables were used to link the 
items together within the queue, and we would update these pointers as we 
expanded and manipulated the queue.

```C
struct queue {
	/* TODO Phase 1 */
	item_t head;
	item_t tail;
	int counter;
};

struct item {
	item_t prev;
	item_t next;
	void *data;
};
```

### Phase 2: Uthread API

In our implementation of thread management, we created two queues, *READY* and 
*BLOCKED*, and two thread states *RUNNING* and *IDLE*. The two queues would 
allow us to store and manipulate the threads for scheduling, and the two state 
threads help us keep track of the threads and which thread should be the one 
being executed.

The basic premise behind our implementation is that we would create and allocate 
space for an idle thread, move it to *IDLE* and *RUNNING* to initialize it. Then 
we create the initial thread, which would go into an infinite while loop that 
only exits when queue lengths is 0, meaning there's no more threads left to run. 
Through using the yield function, we would switch the contexts of the running 
and ready threads to execute it until there's no more threads left to run. As of 
right now, we are only using the *READY* queue, but once we move to phase 3, we 
will use the *BLOCKED* queue as well.


Within our struct, *uthread_tcb*, we have a context and a stackpointer. Upon 
runnning the thread through *uthread_run*. To initialize and set up the 
stackpointer and context, we made use of the functions available to us in 
*context.c* such as `uthread_ctx_switch()` and `uthread_ctx_destroy_stack`.

### Phase 3: Semaphores

For our semaphore implementation, we made use of a count variable within our 
semaphore struct and a waiting queue. The two functions, `sem_up()` and 
`sem_down()` would increment and decrement the counter variable which would 
determine which process we should block and unblock. This essentially serves as 
a lock to account for threads sharing a common resource. We access the common 
resource through `uthread_current()` and use the waiting queue to enqueue and 
dequeue this thread.

### Phase 4: Preemption

Due to time constraints, we weren't able to implement this phase of the project

### Testing
In regards to testing, we ran the sample files provided by the Professor and 
compared our output with the expected output. For queue, we created our own 
tester file based off the professor's tester. We made  test cases that 
accounted for very specific situtations to make sure our program would be able 
to handle edge cases. We also had to make our program account for error cases 
such as when the data is NULL.

In terms of debugging, we had to redo our implementation and rethink our current 
plan for each phase. In most of the phases, especially phase 2, we thought we 
had an adequate understanding of the concept, but the bugs made us realize that 
we were understanding the topic incorrectly and had to consult the slides to 
come up with a new approach. For the really serious bugs, we had to consult 
office hours and utilized the GDB debugger.

### Sources

Throughout the entirety of this project, we consulted the lecture slides, piazza 
posts, office hours, and the demo code provided on class. Our main issue with 
the project came from not understanding the topics properly, so we had to use 
the slides heavily to come up with an implementation.

For our queue implementation, we referenced 
https://www.tutorialspoint.com/data_structures_algorithms/linked_list_program_
in_c.htm

### Conlusion

While we weren't able to complete phase 4 of the assignment, the preemption 
phase, we were satisfied with the other phases and the project as a whole. 
Through this assignment, we were able to get a far more in-depth understanding 
of thread management.

