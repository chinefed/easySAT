#include "stack.h"

void initStack(stack *myStack)
{
    myStack->head = NULL;
}

void push(stack *myStack, void *myData)
{
    /* Pushes a pointer to some data to the stack. */
    stackNode *newNode;
    newNode = (stackNode *)malloc(sizeof(stackNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    newNode->data = myData;
    newNode->previous = myStack->head;
    myStack->head = newNode;
}

void pop(stack *myStack)
{
    /* Pops an element from the stack. */
    if (myStack->head == NULL) {
        fprintf(stderr, "Illegal access to empty stack.\n");
        exit(1);
    }
    stackNode *newHead = (myStack->head)->previous;
    free(myStack->head);
    myStack->head = newHead;
}

void *peek(stack *myStack)
{
    /* Peeks into the stack. */
    if (myStack->head == NULL) {
        fprintf(stderr, "Illegal access to empty stack.\n");
        exit(1);
    }

    return (myStack->head)->data;
}

void clearStack(stack *myStack)
{
    /* Pops all the elements in the stack. */
    while (myStack->head != NULL)
    {
        pop(myStack);
    }
}