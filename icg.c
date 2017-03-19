// David M Allen
// Intermediate Code Generator
// COP 3402 - Systems Software

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "icg.h"
#include "common.h"

void genCode(int op, int r, int l, int m)
{
    // open the file for writing
    FILE *fid;

    fid = fopen(objectCodeFileName, "a");

    // write the new instruction
    fprintf(fid, "%d %d %d %d\n", op, r, l, m);

    // close the file
    fclose(fid);
}
