#include <stdio.h>
#include <assert.h>
#include "../src/queue.h"

void test_enqueue_dequeue(void)
{
    // TEST 1
    printf("TEST 1: enqueue and dequeue\n\n");

    queue *myQueue = initQueue(3);

    enqueue(myQueue, 1);
    enqueue(myQueue, -1);
    enqueue(myQueue, 4);

    assert(dequeue(myQueue) == 1);
    assert(dequeue(myQueue) == -1);
    assert(dequeue(myQueue) == 4);

    freeQueue(myQueue);

    printf("Test 1 passed.\n");
}

void test_auto_resize(void)
{
    // TEST 2
    printf("\n\nTEST 2: autoResize\n");

    queue *myQueue = initQueue(10);

    enqueue(myQueue, 1);
    enqueue(myQueue, 2);
    enqueue(myQueue, 3);
    enqueue(myQueue, 4);
    enqueue(myQueue, 5);
    autoResize(myQueue);
    assert(dequeue(myQueue) == 1);
    assert(dequeue(myQueue) == 2);
    assert(myQueue->head == myQueue->q + 2);
    assert(myQueue->tail == myQueue->q + 4);
    assert(dequeue(myQueue) == 3);
    assert(dequeue(myQueue) == 4);
    assert(dequeue(myQueue) == 5);

    freeQueue(myQueue);

    printf("Test 2 passed.\n");
}

int main(void)
{
    test_enqueue_dequeue();
    test_auto_resize();

    return 0;
}