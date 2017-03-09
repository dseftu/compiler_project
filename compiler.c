// David M Allen
// Compiler-Driver
// COP 3402 - Systems Software

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

// the halt flag.  When set to TRUE, the program ends
int halt = FALSE;

#include "compiler.h"
#include "errorCodes.h"

int main(int argc, char *argv[])
{
    // process input arguments to the compiler
    printf("%s\n", "test");
    // TODO: Remove this error.  This was a test of the errorhandler.
    error(BADCONSTASSIGNMENTSYMBOL);
    // process input via the scanner
    // Pseudo code:
    // lexemeList = scan(inputFilename);
    // either just have the list be in mem or pass it back (probably pass it back)

    if (halt == TRUE) return 0;
    // TODO: Remove this error.  This was a test of the errorhandler.
    error(BADSYMBOLAFTERSTMT);
    // write lexeme list to a file

    // send output of scanner to parser to process
    // Pseudo code:
    // parse(lexemeList)
    // There is no output from parse other than whether the code halted
    if (halt == TRUE) return 0;

    // send output of scanner to icg to generate object code
    // Pseudo code:
    // objectCode = generateObjectCode(lexemelist);
    // write object code to a file

    if (halt == TRUE) return 0;

    // send objectCode to vm to execute
    // Pseudo code:
    // execute(objectCode)


    return 0;
}
