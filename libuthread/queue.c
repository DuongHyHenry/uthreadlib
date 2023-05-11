#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"


//defines a queue with a head node, tail node, and counter
struct queue {
	item_t head;
	item_t tail;
	int counter;
};

//defines a doubly linked list node
struct item {
	item_t prev;
	item_t next;
	void *data;
};

//creates the queue
queue_t queue_create(void)
{

	//creates a new queue and dynamically allocates space for it while initializing counter
	queue_t newQueue;
	newQueue = calloc(1, sizeof(struct queue));
	newQueue->counter = 0; 
	return newQueue;
}

//deallocates queue
int queue_destroy(queue_t queue)
{
	
	//frees the space dynamically allocated by queue
	free(queue);
	queue = NULL;
	return 0;
}

//inserts new element into queue
int queue_enqueue(queue_t queue, void *data)
{
	
	//returns with -1 if queue or data is NULL
	if((queue == NULL) || (data == NULL)){
		return -1;
	}

	//creates a new node and allocates space
	item_t newItem = calloc(1, sizeof(struct queue));

	//initializes data of new item with data from function call
	newItem->data = data;

	//if counter is 0, the queue is empty
	if (queue->counter == 0) {

		//initializes the newItem as the head and tail as it's the only node in the queue
		queue->head = newItem;
		queue->tail = newItem;

		//there's no nodes before or after it
		newItem->prev = NULL;
		newItem->next = NULL;
	}
	else {

		//puts newItem at the last node in the queue
		queue->tail->next = newItem;

		//makes the previous node of newItem the previous last node
		newItem->prev = queue->tail;

		//makes newItem the new tail node
		queue->tail = newItem;
	}

	//increments queue by 1 because of inserting a new node
	queue->counter++;
	return 0;
}

//dequeues first element
int queue_dequeue(queue_t queue, void **data)
{
	
	//if queue, data, or number of nodes in queue is 0, returns with -1
	if((queue==NULL)||(data==NULL)||(queue->counter==0)){
		return -1;
	}

	//if queue only has 1 node
	if (queue->counter == 1) {

		///data takes the data of the first node in queue
		*data = queue->head->data;

		//head and tail are null because there's now 0 nodes in queue
		queue->head = NULL;
		queue->tail = NULL;
	}
	else {
		
		///data takes the data of the first node in queue
		*data = queue->head->data;

		//the queue's head becomes the second node in the queue
		queue->head = queue->head->next;
	}

	//decrements counter by 1 because of taking out a node
	queue->counter--;
	return 0;
}

//deletes an element who's data matches void* data
int queue_delete(queue_t queue, void *data)
{
	
	//allocates space for an "iterator" node newItem
	item_t newItem = calloc(1, sizeof(struct item));

	//newItem points to the queue's head
	newItem = queue->head;

	//while newItem doesn't reach the end of the queue
    while (newItem->data != NULL || newItem != NULL) {

		//if the item we're looking for is the head
        if((newItem==queue->head) && (newItem->data==data)){

			//the queue's next item becomes the head and the head is forgotten
            queue->head=queue->head->next;
			break;
        }

		//if the newItem matches the item we're looking for
        if (newItem->data == data) {

			//if newItem isn't the last element in the queue
			if (newItem->next != NULL) {
 
				/* shifts pointers around so that nothing is pointing 
				at the node newItem is pointing to anymore */
	            newItem->next->prev = newItem->prev;
			}

			//if newItem isn't the first element in the queue
			if (newItem->prev != NULL) {

				/* shifts pointers around so that nothing is pointing 
				at the node newItem is pointing to anymore */
	            newItem->prev->next = newItem->next;
			}

			//decerements the counter because an element has been deleted
			queue->counter--;
			break;
        }

		//iterates to the next item in the queue
        newItem = newItem->next;
    }
    return 0;
}

//iterates through queue executing func
int queue_iterate(queue_t queue, queue_func_t func)
{

	//if queue of func equals NULL, return -1
	if((queue==NULL)||(func==NULL)){
		return -1;
	}

	//allocates space for an "iterator" node newItem
	item_t newItem = calloc(1, sizeof(struct queue));

	//newItem points to the queue's head
	newItem = queue->head;

	//while newItem hasn't reached the end of the queue
	while(newItem!=NULL){

		//execute the function on the current data pointed to by newItem
		func(queue, newItem->data);

		//iterates to the next node in the queue
		newItem=newItem->next;
	}
	return 0;
	
}

//returns the length of queue
int queue_length(queue_t queue)
{
	
	//if queue is null, i.e. not created yet, return 1
	if(queue==NULL){
		return -1;
	} 
	
	//otherwise, return the counter of the queue
	else {
		return queue->counter;
	}
	
}
