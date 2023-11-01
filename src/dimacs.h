#ifndef DIMACS_H
#define DIMACS_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include "solver.h"
#include "bitmap.h"

/**
 * Parses a DIMACS file and stores the clauses in an array of atoms.
 * 
 * @param file The DIMACS file to parse.
 * @return .
 */
atom *parseDimacs(FILE *file, int32_t *nVars, int32_t *nClauses);

#endif