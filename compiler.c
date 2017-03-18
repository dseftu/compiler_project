// David M Allen
// Compiler-Driver
// COP 3402 - Systems Software

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "common.h"
#include "compiler.h"
#include "errorCodes.h"

// the halt flag.  When set to TRUE, the program ends
int halt = FALSE;

int main(int argc, char *argv[])
{
    // process input arguments to the compiler
    
    // set flags as appropriate dependend on args
    
    
    // process input via the scanner
    // Pseudo code:
    // lexemeList = scan(inputFilename);
    // either just have the list be in mem or pass it back (probably pass it back)
    int lexemeListMaxIndex = 0;
    lexeme* lexemeList;
    lexemeList = scan(argv[1], &lexemeListMaxIndex);
    if (halt == TRUE) return 0;
    
    // TODO:  Make this print be optional depending on args
    printLexemeList(lexemeList, lexemeListMaxIndex);
    if (halt == TRUE) return 0;
    // write lexeme list to a file?

    // send output of scanner to parser to process
    // Pseudo code:
    parse(lexemeList, lexemeListMaxIndex);
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
