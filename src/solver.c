#include "solver.h"
#include <assert.h>

typedef struct State {
    int32_t x; /**< Var evaluated at the current state. */
    int32_t litID;
    bitmap *evalStatus; /**< Evaluation status (true clauses). */
    bitmap *varStatus; /**< Assigned variables. */
    bitmap *varSign; /**< Polarity of the assignment. */
    queue *searchQueue; /**< Literals for generating assignments. */
} state;

typedef struct NextCls {
    int32_t index; /**< Index of the next clause to analyze. */;
    int32_t freedom; /**< Freedom of the next clause. */
} nextCls;

state *initState(int32_t nVars, int32_t nClauses);
void freeState(state *myState);
void popState(stack *stateStack);
void clearStateStack(stack *stateStack);
void baseCase(stack *stateStack, atom *variables, atom *clauses);
char solverStep(stack *stateStack, atom *variables, atom *clauses);
void manageConflict(stack *stateStack, state *topState, atom *variables);
queue *getSearchQueue(atom *clauses, bitmap *evalStatus, bitmap *varStatus);
nextCls getNextClause(atom *clauses, bitmap *evalStatus, bitmap *varStatus);
int32_t getFreedom(atom *clause, bitmap *varStatus);
void pushToSearchQueue(queue *searchQueue, atom *clause, bitmap *varStatus);
void printAssignment(bitmap *varStatus, bitmap *varSign);

atom *initAtomArray(uint32_t n, uint32_t m)
{
    /* Allocates an array of atoms. */
    atom *myAtomArray;
    myAtomArray = (atom *)allocate1DArray(n, sizeof(atom));
    for (int i = 0; i < n; i++) {
        myAtomArray[i].X = initBitmap(m);
        myAtomArray[i].notX = initBitmap(m);
    }
    return myAtomArray;
}

void freeAtomArray(atom *myAtomArray, uint32_t n)
{
    /* Deallocates an array of atoms. */
    for (int i = 0; i < n; i++) {
        freeBitmap(myAtomArray[i].X);
        freeBitmap(myAtomArray[i].notX);
    }
    free1DArray((void *)myAtomArray);
}

state *initState(int32_t nVars, int32_t nClauses)
{
    /* Allocates a state. */
    state *myState;
    myState = (state *)malloc(sizeof(state));
    myState->x = -1;
    myState->evalStatus = initBitmap(nClauses);
    myState->varStatus = initBitmap(nVars);
    myState->varSign = initBitmap(nVars);
    myState->searchQueue = NULL;

    return myState;
}

void freeState(state *myState)
{
    /* Deallocates a state. */
    freeBitmap(myState->evalStatus);
    freeBitmap(myState->varStatus);
    freeBitmap(myState->varSign);
    if (myState->searchQueue != NULL) {
        freeQueue(myState->searchQueue);
    }
    free(myState);
}

void popState(stack *stateStack)
{
    /* Pops the top state from the state stack. */
    freeState(peek(stateStack));
    pop(stateStack);
}

void clearStateStack(stack *stateStack)
{
    /* Pops the top state from the state stack. */
    while (stateStack->head != NULL) {
        popState(stateStack);
    }
}

void solver(atom *variables, atom *clauses)
{
    /* Solver algorithm. */

    stack stateStack;
    initStack(&stateStack);

    // Base case
    baseCase(&stateStack, variables, clauses);

    char step = 0;
    uint32_t count = 0;
    while ((step != 1) && (stateStack.head != NULL)) {
        step = solverStep(&stateStack, variables, clauses);
        count++;
    }

    printf("\033[2K\r");
    printf("\nSolution found in %" PRIu32 " solver steps.\n", count);

    if (step) {
        state *topState;
        topState = peek(&stateStack);
        int32_t nVars = clauses->X->nBits;
        int32_t setVars = countSetBits(topState->varStatus);
        printf("\nSATISFIABLE (%" PRId32 " assigned variables, %" PRId32 " free variables)\n", setVars, nVars-setVars);
        printAssignment(topState->varStatus, topState->varSign);
    }
    else {
        printf("UNSATISFIABLE\n");
    }

    clearStateStack(&stateStack);
}

void baseCase(stack *stateStack, atom *variables, atom *clauses)
{
    int32_t nVars = clauses->X->nBits;
    int32_t nClauses = variables->X->nBits;
    state *baseState;
    baseState = initState(nVars, nClauses);
    baseState->x = -1;
    baseState->litID = 0;
    baseState->searchQueue = getSearchQueue(clauses, baseState->evalStatus, baseState->varStatus);
    push(stateStack, baseState);
}

char solverStep(stack *stateStack, atom *variables, atom *clauses)
{
    /* Step of the solver algorithm. */
    state *topState;
    topState = peek(stateStack);

    if (topState->searchQueue->n == 0) {
        // Conflict detected
        manageConflict(stateStack, topState, variables);
        return 0;
    }

    int32_t nVars = clauses->X->nBits;
    int32_t nClauses = variables->X->nBits;

    state *newState;
    newState = initState(nVars, nClauses);
    newState->litID = dequeue(topState->searchQueue);
    newState->x = (newState->litID > 0) ? (newState->litID-1) : (-newState->litID-1);

    // Copy status bitmap from topState
    bitmapOR(newState->evalStatus, topState->evalStatus);
    bitmapOR(newState->varStatus, topState->varStatus);
    bitmapOR(newState->varSign, topState->varSign);

    // We want to make true clauses that include litID.
    bitmap *litBitmap;
    // 1) if it has positive polarity ("x", positive litID), we assign TRUE to x;
    // 2) if it has negative polarity ("not x", negative litID), we assign FALSE to x.
    litBitmap = (newState->litID > 0) ? variables[newState->litID-1].X : variables[-newState->litID-1].notX;
    // The bits set in "newState->evalStatus" are the ones set in "topState->evalStatus"
    // + some additional ones corresponding to the clauses that have become
    // true following the current assignment.
    bitmapOR(newState->evalStatus, litBitmap);

    // Update varStatus and varSign bitmaps
    switchBit(newState->varStatus, newState->x);
    if (newState->litID < 0) {
        // 0 : x, 1 : -x
        switchBit(newState->varSign, newState->x);
    }

    // Get the search queue
    newState->searchQueue = getSearchQueue(clauses, newState->evalStatus, newState->varStatus);

    // Push the newState to the Stack
    push(stateStack, newState);

    // Update status bar
    int32_t satClauses = countSetBits(newState->evalStatus);
    updateStatusBar(satClauses, nClauses);

    if (satClauses == nClauses) {
        // If all the clauses are sat
        return 1;
    }

    // If there are unsat clauses
    return 0;
}

void manageConflict(stack *stateStack, state *topState, atom *variables)
{
    /* Manage conflict. */
    int32_t x = topState->x;
    int32_t litID = -topState->litID;
    popState(stateStack);
    if (stateStack->head == NULL) {
        // The formula is unsat
        return;
    }
    topState = peek(stateStack);
    switchBit(topState->varStatus, x);
    if (litID < 0) {
        switchBit(topState->varSign, x);
    }
    bitmap *litBitmap;
    litBitmap = (litID > 0) ? variables[litID-1].X : variables[-litID-1].notX;
    bitmapOR(topState->evalStatus, litBitmap);
}

queue *getSearchQueue(atom *clauses, bitmap *evalStatus, bitmap *varStatus)
{
    /* Builds the search queue. */
    nextCls nextClause = getNextClause(clauses, evalStatus, varStatus);
    if (nextClause.index == -1) {
        return NULL;
    }

    queue *searchQueue;
    searchQueue = initQueue(nextClause.freedom);

    pushToSearchQueue(searchQueue, clauses+nextClause.index, varStatus);

    return searchQueue;
}

nextCls getNextClause(atom *clauses, bitmap *evalStatus, bitmap *varStatus)
{
    /* Determines the next caluse to evaluate
    (i.e., the clause with the lowest freedom). */
    nextCls nextClause;
    nextClause.index = -1;
    nextClause.freedom = INT32_MAX;
    for (int32_t i = 0; i < evalStatus->nBits; i++) {
        if (!readBitmap(evalStatus)) {
            int32_t freedom = getFreedom(clauses+i, varStatus);
            if (freedom < nextClause.freedom) {
                nextClause.index = i;
                nextClause.freedom = freedom;
            }
        }
    }

    return nextClause;
}

int32_t getFreedom(atom *clause, bitmap *varStatus)
{
    /* Computes the number of free variables for a clause
    accounting for "varStatus". */
    int32_t freedom = 0;
    for (int32_t w = 0; w < varStatus->nWords; w++) {
        freedom += countSetBitsInWord((clause->X->map)[w] & (~varStatus->map[w]));
        freedom += countSetBitsInWord((clause->notX->map)[w] & (~varStatus->map[w]));
    }
    
    return freedom;
}

void pushToSearchQueue(queue *searchQueue, atom *clause, bitmap *varStatus)
{
    /* Pushes literals to the search queue for evaluation. */
    int32_t j = 1;
    for (int32_t w = 0; w < varStatus->nWords; w++) {
        uint64_t adjWordX = (clause->X->map)[w] & (~varStatus->map[w]);
        uint64_t adjWordNotX = (clause->notX->map)[w] & (~varStatus->map[w]);
        char i = 0;
        while ((i < 64) && (j <= varStatus->nBits)) {
            if ((adjWordX >> i) & 1) {
                enqueue(searchQueue, j);
            }
            if ((adjWordNotX >> i) & 1) {
                enqueue(searchQueue, -j);
            }
            i++;
            j++;
        }
    }
}

void printAssignment(bitmap *varStatus, bitmap *varSign)
{
    /* Prints the truth assignment */
    int32_t nVars = varStatus->nBits;
    int32_t *buffer = (int32_t *)allocate1DArray(nVars, sizeof(int32_t));

    // Get var IDs from bitmap
    for (int32_t i = 0; i < nVars; i++) {
        if (readBitmap(varStatus)) {
            buffer[i] = i+1;
        }
    }

    // Get signs from bitmap
    for (int32_t i = 0; i < nVars; i++) {
        if (readBitmap(varSign)) {
            buffer[i] = -buffer[i];
        }
    }

    // Print
    for (int32_t i = 0; i < nVars; i++) {
        if (buffer[i] != 0) {
            printf("%" PRId32 " ", buffer[i]);
        }
    }

    free1DArray(buffer);
}
