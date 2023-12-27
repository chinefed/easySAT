#include "solver.h"
#include <assert.h>

typedef struct State {
    int32_t x; /**< Var evaluated at the current state. */
    int32_t litID;
    bitmap *evalStatus; /**< Evaluation status (true clauses). */
    queue *searchQueue; /**< Literals for generating assignments. */
} state;

typedef struct NextCls {
    int32_t index; /**< Index of the next clause to analyze. */;
    int32_t freedom; /**< Freedom of the next clause. */
} nextCls;

state *initState();
void freeState(state *myState);
void popState(stack *stateStack);
void clearStateStack(stack *stateStack);
void baseCase(stack *stateStack, atom *variables, atom *clauses, bitmap *varStatus);
char solverStep(stack *stateStack, atom *variables, atom *clauses, bitmap *varStatus);
bitmap *getNewEvalStatus(bitmap *evalStatus, atom *variables, int32_t litId);
queue *getSearchQueue(atom *clauses, bitmap *evalStatus, bitmap *varStatus);
nextCls getNextClause(atom *clauses, bitmap *evalStatus, bitmap *varStatus);
int32_t getFreedom(atom *clause, bitmap *varStatus);
void pushToSearchQueue(queue *searchQueue, atom *clause, bitmap *varStatus);

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

state *initState()
{
    /* Allocates a state. */
    state *myState;
    myState = (state *)malloc(sizeof(state));
    myState->x = -1;
    myState->evalStatus = NULL;
    myState->searchQueue = NULL;

    return myState;
}

void freeState(state *myState)
{
    /* Deallocates a state. */
    freeBitmap(myState->evalStatus);
    freeQueue(myState->searchQueue);
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
    while (stateStack->head != NULL)
    {
        state *topState;
        topState = peek(stateStack);
        printf("%d ", topState->litID);
        popState(stateStack);
    }
}

void solver(atom *variables, atom *clauses)
{
    /* Solver algorithm. */

    int32_t nVars = clauses->X->nBits;
    bitmap *varStatus;
    varStatus = initBitmap(nVars);

    stack stateStack;
    initStack(&stateStack);

    // Base case
    baseCase(&stateStack, variables, clauses, varStatus);

    char step = 0;
    uint32_t count = 0;
    while ((step != 1) && (stateStack.head != NULL)) {
        step = solverStep(&stateStack, variables, clauses, varStatus);
        count++;
    }

    if (step) {
        int32_t setVars = countSetBits(varStatus);
        printf("\nSolution found in %d iterations.\n", count);
        printf("SATISFIABLE (%d assigned variables, %d free variables)\n", setVars, nVars-setVars);
    } else {
        printf("\033[2K\r");
        printf("UNSATISFIABLE\n");
    }

    freeBitmap(varStatus);
    clearStateStack(&stateStack);

}

void baseCase(stack *stateStack, atom *variables, atom *clauses, bitmap *varStatus)
{
    int32_t nClauses = variables->X->nBits;
    state *baseState;
    baseState = initState();
    baseState->x = -1;
    baseState->litID = 0;
    baseState->evalStatus = initBitmap(nClauses);
    baseState->searchQueue = getSearchQueue(clauses, baseState->evalStatus, varStatus);
    push(stateStack, baseState);
}

char solverStep(stack *stateStack, atom *variables, atom *clauses, bitmap *varStatus)
{
    /* Step of the solver algorithm. */
    state *topState;
    topState = peek(stateStack);

    if (topState->searchQueue->n == 0) {
        // Conflict detected
        if (topState->x >= 0) {
            // Revert varStatus update
            switchBit(varStatus, topState->x);
        }
        // Pop state from the stack
        popState(stateStack);
        return 0;
    }

    int32_t litId = dequeue(topState->searchQueue);
    int32_t x = (litId > 0) ? (litId-1) : (-litId-1);
    switchBit(varStatus, x); // Update varStatus

    bitmap* newEvalStatus;
    newEvalStatus = getNewEvalStatus(topState->evalStatus, variables, litId);
    // If the number of bits set in the bitmap is equal to nBits, returns 1
    int32_t satClauses = countSetBits(newEvalStatus);
    int32_t nClauses = variables->X->nBits;
    if (satClauses == newEvalStatus->nBits) {
        updateStatusBar(satClauses, nClauses);
        return 1;
    }

    queue *searchQueue;
    searchQueue = getSearchQueue(clauses, newEvalStatus, varStatus);
    if (searchQueue->n == 0) {
        // Conflict detected
        // Revert varStatus update
        switchBit(varStatus, x);
        return 0;
    }

    state *newState;
    newState = initState();
    newState->x = x;
    newState->litID = litId;
    newState->evalStatus = newEvalStatus;
    newState->searchQueue = searchQueue;
    push(stateStack, newState);
    updateStatusBar(satClauses, nClauses);

    return 0;
}

bitmap *getNewEvalStatus(bitmap *evalStatus, atom *variables, int32_t litId)
{
    /* Get updated eval status following a truth assignment. */
    // The function receives as input a literal ID.
    // We want to make true clauses that include such a literal: 
    // 1) if it has positive polarity ("x", positive literal ID), we assign TRUE to x;
    // 2) if it has negative polarity ("not x", negative literal ID), we assign FALSE to x.
    bitmap *newEvalStatus;
    newEvalStatus = initBitmap(evalStatus->nBits);
    bitmap *litBitmap;
    // Get the bitmap with the clauses made true by the assignment.
    litBitmap = (litId > 0) ? variables[litId-1].X : variables[-litId-1].notX;
    int32_t nWords =  getWordIdx(evalStatus->nBits-1)+1;
    for (int32_t i = 0; i < nWords; i++) {
        // The bits set in "newEvalStatus" are the ones set in "evalStatus"
        // + some additional ones corresponding to the clauses that have become
        // true following the current assignment.
        newEvalStatus->map[i] = evalStatus->map[i] | litBitmap->map[i];
    }

    return newEvalStatus;
}

queue *getSearchQueue(atom *clauses, bitmap *evalStatus, bitmap *varStatus)
{
    /* Builds the search queue. */
    nextCls nextClause = getNextClause(clauses, evalStatus, varStatus);
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
