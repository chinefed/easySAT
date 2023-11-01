#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "../src/dimacs.h"
#include "../src/solver.h"
#include "../src/bitmap.h"

void test1(void)
{
    // TEST 1
    printf("TEST 1\n\n");

    DIR *dir;
    struct dirent *entry;
    const char *directory_path = "./sample";
    dir = opendir(directory_path);
    if (dir == NULL) {
        perror("opendir");
        exit(1);
    }

    FILE *file;
    int i = 1;
    while ((entry = readdir(dir)) != NULL) {

        if (entry->d_type == DT_REG) {

            char file_path[1024];
            snprintf(file_path, sizeof(file_path), "%s/%s", directory_path, entry->d_name);
            file = fopen(file_path, "r");
            if (file == NULL) {
                fprintf(stderr, "Error opening the file.\n");
                exit(1);
            }

            int32_t nVars, nClauses;
            atom *clauses;
            clauses = parseDimacs(file, &nVars, &nClauses);
            freeAtomArray(clauses, nClauses);
            printf("Test instance %d parsed.\n", i);

            fclose(file);
            i++;
        }
    }

    closedir(dir);
}

void test2(void)
{
    // TEST 2
    printf("\n\nTEST 2\n");

    const char* test = "c ab\nc def\np cnf 10 3\n0 1 -1 0 1 2 3 0\n-1 2 -3";
    FILE* file2 = fmemopen((void*)test, strlen(test), "r");
    int32_t nVars, nClauses;
    atom *clauses;
    clauses = parseDimacs(file2, &nVars, &nClauses);
    for (int i = 0; i < nClauses; i++) {
            printf("Clause %d - X\n", i);
            printBitmap(clauses[i].X);
            printf("Clause %d - notX\n", i);
            printBitmap(clauses[i].notX);
    }
}

int main(void)
{
    
    // Test 1
    test1();

    // Test 2
    //test2();

    return 0;
}
