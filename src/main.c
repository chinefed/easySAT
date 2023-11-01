#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "dimacs.h"
#include "solver.h"

int main(int argc, char *argv[])
{
    FILE *file;
    file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening the file.\n");
        exit(1);
    }
    
    int32_t nVars, nClauses;
    atom *clauses;
    clauses = parseDimacs(file, &nVars, &nClauses);
    fclose(file);
    
    atom *variables;
    variables = initAtomArray(nVars, nClauses);
    for (int i = 0; i < nClauses; i++) {
        for (int j = 0; j < nVars; j++) {
            if (readBitmap(clauses[i].X)) {
                switchBit(variables[j].X, i);
            }
        }
        for (int j = 0; j < nVars; j++) {
            if (readBitmap(clauses[i].notX)) {
                switchBit(variables[j].notX, i);
            }
        }
    }

    solver(variables, clauses);

    freeAtomArray(clauses, nClauses);
    freeAtomArray(variables, nVars);

    return 0;
}


