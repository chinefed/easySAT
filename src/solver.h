#ifndef SOLVER_H
#define SOLVER_H

#include <stdlib.h>
#include <stdint.h>
#include "bitmap.h"
#include "utils.h"
#include "stack.h"
#include "queue.h"

/**
 * @brief A struct representing a logical atom in the SAT solver.
 * 
 * An atom is defined by two bitmaps: one for the atom itself and one for its negation.
 * 
 */
typedef struct Atom {
    bitmap *X;
    bitmap *notX;
} atom;

/**
 * Allocates an array of atoms.
 * 
 * @param n The number of atoms to allocate.
 * @param m The length of the bitmaps in each atom.
 * 
 * @return A pointer to the allocated array of atoms.
 */
atom *initAtomArray(uint32_t n, uint32_t m);

/**
 * Deallocates an array of atoms.
 * 
 * @param myAtomArray The array of atoms to deallocate.
 * @param n The number of atoms in the array.
 */
void freeAtomArray(atom *myAtomArray, uint32_t n);

/**
 * The solver function implements the SAT solver algorithm.
 * 
 * @param variables An array of atoms representing the variables in the formula.
 * @param clauses An array of atoms representing the clauses in the formula.
 */
void solver(atom *variables, atom *clauses);

#endif



