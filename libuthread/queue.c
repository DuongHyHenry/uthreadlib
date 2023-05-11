#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"



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

queue_t queue_create(void)
{
	/* TODO Phase 1 */
	queue_t newQueue;
	newQueue = calloc(1, sizeof(struct queue));
	newQueue->counter = 0; 
	return newQueue;
}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
	free(queue);
	queue = NULL;
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	if((queue==NULL)||(data==NULL)){
		return -1;
	}
	item_t newItem = calloc(1, sizeof(struct queue));
	newItem->data = data;
	if (queue->counter == 0) {
		queue->head = newItem;
		queue->tail = newItem;
		newItem->prev = NULL;
		newItem->next = NULL;
	}
	else {
		queue->tail->next = newItem;
		newItem->prev = queue->tail;
		queue->tail = newItem;
	}
	queue->counter++;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
	if((queue==NULL)||(data==NULL)||(queue->counter==0)){
		return -1;
	}
	if (queue->counter == 1) {
		*data = queue->head->data;
		queue->head = NULL;
		queue->tail = NULL;
	}
	else {
		*data = queue->head->data;
		queue->head = queue->head->next;
	}
	queue->counter--;
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	item_t newItem = calloc(1, sizeof(struct item));
	newItem = queue->head;
    while (newItem->data != NULL || newItem != NULL) {
        if((newItem==queue->head) && (newItem->data==data)){
            queue->head=queue->head->next;
			break;
        }
        if (newItem->data == data) {
			if (newItem->next != NULL) {
	            newItem->next->prev = newItem->prev;
			}
			if (newItem->prev != NULL) {
	            newItem->prev->next = newItem->next;
			}
			queue->counter--;
			break;
        }

        newItem = newItem->next;
    }
    return 0;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
	item_t newItem = calloc(1, sizeof(struct queue));
	newItem = queue->head;
	while(newItem!=NULL){
		func(queue, newItem->data);
		newItem=newItem->next;
	}
	if((queue==NULL)||(func==NULL)){
		return -1;
	}
	return 0;
	
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
	if(queue==NULL){
		return -1;
	}else{
		return queue->counter;
	}
	
}
