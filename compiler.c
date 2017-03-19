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
    
    // creates the string to be used for the object code filename
    setObjectCodeFileName(argv[1]);    
    
    // process input via the scanner
    int lexemeListMaxIndex = 0;
    lexeme* lexemeList;
    lexemeList = scan(argv[1], &lexemeListMaxIndex);
    if (halt == TRUE) return 0;
    
    // TODO:  Make this print be optional depending on args
    printLexemeList(lexemeList, lexemeListMaxIndex);
    if (halt == TRUE) return 0;
    
    // send output of scanner to parser to process
    // this should also generate the object code
    parse(lexemeList, lexemeListMaxIndex);

    // There is no output from parse other than whether the code halted
    if (halt == TRUE) return 0;

    // send objectCode to vm to execute
    run(objectCodeFileName);

    return 0;
}

void setObjectCodeFileName(char* inputFile)
{
    objectCodeFileName = malloc(strlen(inputFile)+1);
    strcpy(objectCodeFileName, inputFile);
    
    for (int i = strlen(objectCodeFileName); i >= 0; i--)
    {
        if (objectCodeFileName[i] == '.')
        {
            objectCodeFileName[i+1] = 'o';
            objectCodeFileName[i+2] = '\0';
            break;
        }
    }
    
}



