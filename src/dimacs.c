#include "dimacs.h"

void parseComments(FILE *file);
void parseProblemLine(FILE *file, int32_t *nVars, int32_t *nClauses);
void emptyClsDetect(int32_t x);

void parseComments(FILE *file)
{
    /* Moves the file pointer up to the problem line. */
    char ch, chLag;
    ch = chLag = '\n';
    while ((ch = fgetc(file)) != EOF) {
        if ((chLag == '\n') && (ch == 'p')) {
            break;
        }
        chLag = ch;
    }
}

void parseProblemLine(FILE *file, int32_t *nVars, int32_t *nClauses)
{
    /* Parses the problem line. */
    char format[4];
    int64_t v, c;
    v = c = -1;

    if (fscanf(file, " %s %" SCNd64 " %" SCNd64, format, &v, &c) != 3) {
        fprintf(stderr, "Error parsing the file.\n");
        exit(1);
    }

    if ((v <= 0) || (c <= 0) || (v > INT32_MAX) || (v > INT32_MAX)) {
        fprintf(stderr, "Error parsing the file.\n");
        exit(1);
    }

    if (strcmp(format, "cnf") != 0) {
        fprintf(stderr, "Unknown format in problem line. Format must be cnf.\n");
        exit(1);
    }

    *nVars = (int32_t)v;
    *nClauses = (int32_t)c;
}

void emptyClsDetect(int32_t x)
{
    /* Detects empty caluses. */
    static int64_t xLag = INT64_MAX;
    if ((x == 0) && (xLag == 0)) {
        printf("Empty clause detected.\n");
        printf("UNSATISFIABLE\n");
        exit(0);
    }
    xLag = x;
}

atom *parseDimacs(FILE *file, int32_t *nVars, int32_t *nClauses) {

    /* Parses DIMACS file. */
    parseComments(file);
    parseProblemLine(file, nVars, nClauses);
    // Allocate an atom array for storing clauses
    atom *clauses = initAtomArray(*nClauses, *nVars);

    int32_t x;
    int32_t clsNum, i;
    clsNum = i = 0;
    while (fscanf(file, " %" SCNd32, &x) == 1) {
        if (clsNum >= *nClauses) {
            fprintf(stderr, "Wrong number of clauses in the problem line.\n");
            exit(1);
        }
        // Detect empty clauses
        emptyClsDetect(x);
        // In case a 0 is parsed, increases the clause counter
        if (x == 0) {
            // Discard the 0 if it precedes the frist clause
            if (i > 0) {
                clsNum++;
            }
            i++;
            continue;
        }
        // Removes the sign
        int32_t ux = (x > 0) ? x : -x;
        if (ux > *nVars) {
            fprintf(stderr, "Wrong number of variables in the problem line.\n");
            exit(1);
        }
        // Store in the atom array
        if (x > 0) {
            // DIMACS counts variables starting from 1
            switchBit(clauses[clsNum].X, ux-1);
        } else {
            switchBit(clauses[clsNum].notX, ux-1);
        }
        i++;
    }

    // If the last value parsed is a 0, discards it
    if (x == 0) {
        clsNum--;
    }

    // Compares the number of clauses detected with the problem line specification
    if ((clsNum+1) != *nClauses) {
        printf("Warning: %" PRIu32 "clauses detected"
        "(problem line specifies %" PRIu32 ")\n", clsNum, *nClauses);
    }

    return clauses;
}