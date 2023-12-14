#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "queue.h"

/**
 * @brief A struct representing a node in a stack.
 * 
 */
typedef struct StackNode stackNode;
typedef struct StackNode {
    void *data; /**< A pointer to some data. */
    stackNode *previous; /**< A pointer to the previous node in the stack. */
} stackNode;

/**
 * @brief  A struct representing a stack.
 * 
 * This struct holds a pointer to the top node of the stack.
 */
typedef struct Stack {
    int64_t size; /**< The stack size. */
    stackNode *head; /**< A pointer to the top node of the stack. */
} stack;

/**
 * Initializes a stack data structure.
 * 
 * @param myStack Pointer to the stack to be initialized.
 */
void initStack(stack *myStack);

/**
 * Pushes a pointer to some data to the stack.
 *
 * @param myStack Pointer to the stack to push the data to.
 * @param myData Pointer to the data to push to the stack.
 */
void push(stack *myStack, void *myData);

/**
 * Pops an element from the stack.
 * 
 * @param myStack Pointer to the stack to pop from.
 * 
 * If the stack is empty, an error message is printed to stderr and the program exits.
 * Otherwise, the top element of the stack is removed and the new top element is updated.
 */
void pop(stack *myStack);

/**
 * Peeks into the stack.
 * 
 * @param myStack The stack to peek into.
 * @return A pointer to the data stored at the top of the stack.
 * If the stack is empty, an error message is printed to stderr and the program exits.
 */
void *peek(stack *myStack);

/**
 * @brief Clears all the elements in the given stack.
 * 
 * @param myStack Pointer to the stack to be cleared.
 */
void clearStack(stack *myStack);

#endif