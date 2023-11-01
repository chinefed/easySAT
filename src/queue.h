#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdint.h>
#include "utils.h"

/**
 * @brief A struct representing a queue data structure.
 * 
 */
typedef struct Queue {
    int32_t n;      /**< The number of elements stored in the queue. */
    int32_t *q;     /**< The array that holds the elements of the queue. */
    int32_t *head;  /**< A pointer to the head of the queue. */
    int32_t *tail;  /**< A pointer to the tail of the queue. */
} queue;

/**
 * Initializes a queue with a given maximum size.
 * 
 * @param maxSize The maximum size of the queue.
 * @return A pointer to the initialized queue.
 */
queue *initQueue(int32_t maxSize);

/**
 * Deallocates a queue.
 * 
 * @param myQueue - the queue to deallocate.
 */
void freeQueue(queue *myQueue);

/**
 * Resizes the array underlying a queue to the exact number of items in the queue.
 * 
 * @param myQueue Pointer to the queue to be resized.
 */
void autoResize(queue *myQueue);

/**
 * Enqueues an integer key to the given queue.
 * 
 * @param myQueue The queue to enqueue the key to.
 * @param key The integer key to enqueue.
 * 
 * @note No error handling in case of full queue. Initialize the queue with an adequate size.
 */
void enqueue(queue *myQueue, int32_t key);

/**
 * Dequeues one integer key from the given queue.
 * 
 * @param myQueue The queue to dequeue from.
 * @return The dequeued integer key.
 * @throws An error message and exits the program if the queue is empty.
 */
int64_t dequeue(queue *myQueue);

#endif