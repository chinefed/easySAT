#include "queue.h"

queue *initQueue(int32_t maxSize)
{
    /* Initializes a queue. */
    queue *myQueue;
    myQueue = (queue *)malloc(sizeof(queue));
    if (myQueue == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    myQueue->n = 0;
    myQueue->q = (int32_t *)allocate1DArray(maxSize, sizeof(int32_t));
    myQueue->head = myQueue->q;
    myQueue->tail = myQueue->q;

    return myQueue;
}

void freeQueue(queue *myQueue)
{
    /* Deallocates a queue. */
    free1DArray(myQueue->q);
    free(myQueue);
}

void autoResize(queue *myQueue)
{
    /* Resizes the array undelying a queue to
    the exact number of items in the queue. */
    int32_t *qNew, *headNew, *tailNew;
    qNew = (int32_t *)allocate1DArray(myQueue->n, sizeof(int32_t));
    for (int32_t i = 0; i < myQueue->n; i++) {
        qNew[i] = myQueue->q[i];
    }
    headNew = qNew + (myQueue->head - myQueue->q);
    tailNew = qNew + (myQueue->n - 1);
    free(myQueue->q);
    myQueue->q = qNew;
    myQueue->head = headNew;
    myQueue->tail = tailNew;
}

void enqueue(queue *myQueue, int32_t key)
{
    /* Enqueues an integer key to the given queue. */
    myQueue->n++;
    *(myQueue->tail++) = key;
}

int64_t dequeue(queue *myQueue)
{
    /* Dequeues one integer key from the given queue. */
    if (myQueue->n > 0) {
        myQueue->n--;
        return *(myQueue->head++);
    } else {
        fprintf(stderr, "Illegal access to empty queue.\n");
        exit(1);
    }
}