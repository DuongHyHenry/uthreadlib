#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	//creates queue
	q = queue_create();

	//enqueues temp variable
	int temp = queue_enqueue(q, &data);

	//dequeues it for testing
	queue_dequeue(q, (void**)&ptr);

	//tests if data is still 3
	TEST_ASSERT(ptr == &data);

	//tests if enqueue worked
    TEST_ASSERT(temp == 0);

	//tests if destroy worked
	TEST_ASSERT(queue_destroy(q) == 0);

	//assigns it to NULL
	q = NULL;

	//tests if destroy didn't work on a NULL queue
	TEST_ASSERT(queue_destroy(q) == -1);
}

int main(void)
{
	test_create();
	test_queue_simple();

	return 0;
}
