#include <stdio.h>
#include <stdlib.h>
#include "../src/stack.h"

void test_push_pop(void)
{
    // TEST 1
    printf("TEST 1: push and pop\n\n");

    stack myStack;
    initStack(&myStack);

    int a = 1;
    int b = 2;
    int c = 3;

    push(&myStack, &a);
    push(&myStack, &b);
    push(&myStack, &c);

    int *popped_c = (int *)peek(&myStack);
    printf("Peeked value: %d\n", *popped_c);

    pop(&myStack);
    int *popped_b = (int *)peek(&myStack);
    printf("Peeked value: %d\n", *popped_b);

    pop(&myStack);
    int *popped_a = (int *)peek(&myStack);
    printf("Peeked value: %d\n", *popped_a);

    pop(&myStack);
    if (myStack.head != NULL) {
        fprintf(stderr, "Error: stack not empty after popping all elements.\n");
        exit(1);
    }

    printf("Test 1 passed.\n");
}

void test_clear(void)
{
    // TEST 2
    printf("\n\nTEST 2: clearStack\n");

    stack myStack;
    myStack.head = NULL;

    int a = 1;
    int b = 2;
    int c = 3;

    push(&myStack, &a);
    push(&myStack, &b);
    push(&myStack, &c);

    clearStack(&myStack);

    if (myStack.head != NULL) {
        fprintf(stderr, "Error: stack not empty after clearing.\n");
        exit(1);
    }

    printf("Test 2 passed.\n");
}

int main(void)
{
    test_push_pop();
    test_clear();

    return 0;
}