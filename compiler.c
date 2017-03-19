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

// argument flags
int printLexemes = FALSE;
int printObjectCode = FALSE;
int printVMExecutionTrace = FALSE;
int inputFileIndex = 1;

// the expected format for input arguments
const char* argL = "-l";
const char* argA = "-a";
const char* argV = "-v";

int main(int argc, char *argv[])
{
    // validate input arguments
    processInputArguments(argc, argv);  
    if (halt == TRUE) return 0;

    // initialize the new object code file
    initObjectCodeFile();
    
    // process input via the scanner
    int lexemeListMaxIndex = 0;
    lexeme* lexemeList;
    lexemeList = scan(argv[inputFileIndex], &lexemeListMaxIndex);
    if (halt == TRUE) return 0;
    
    // print lexeme list if the flag is set
    if (printLexemes == TRUE) 
        printLexemeList(lexemeList, lexemeListMaxIndex);
    if (halt == TRUE) return 0;
    
    // send output of scanner to parser to process
    // this should also generate the object code
    parse(lexemeList, lexemeListMaxIndex);
    if (halt == TRUE) return 0;

    // send objectCode to vm to execute
    run(objectCodeFileName);

    return 0;
}

void processInputArguments(int argc, char *argv[])
{
    // process input arguments to the compiler
    if (argc < 2)
    {
        error(MISSINGINPUTARGUMENTS);
    }
    else if (argc == 2)
    {
        // if this argument is any of the optional arguments, then this
        // is not valid.  If there is only one argument, we expect a filename
        if (strcmp(argv[1],argL) == 0 ||
            strcmp(argv[1],argA) == 0 ||
            strcmp(argv[1],argV) == 0)
            error(INVALIDINPUTARGUMENTS);
        else
            setObjectCodeFileName(argv[1]);
    }
    else if (argc > 5)
    {
        error(TOOMANYINPUTARGUMENTS);
    }
    else
    {
        // The last arg should be the filename.  If it isn't, then
        // it won't open and that will throw an error.  I'm not
        // going to check to see if it is a valid file here.
        inputFileIndex = argc-1;
        setObjectCodeFileName(argv[argc-1]);

        // iterate through the rest of arg list and figure out what flags need to be set
        int i;
        for (int i = 1; i < argc-1; i++)
        {
            // is this a valid argument at all?
             if (strcmp(argv[i],argL) != 0 &&
                strcmp(argv[i],argA) != 0 &&
                strcmp(argv[i],argV) != 0)
            {
                error(INVALIDINPUTARGUMENTS);
            }                
            else
            {
                if (strcmp(argv[i],argL) == 0) 
                {
                    // make sure we didn't have duplicate flags
                    if (printLexemes == FALSE) printLexemes = TRUE;
                    else error(INVALIDINPUTARGUMENTS);
                }
                if (strcmp(argv[i],argA) == 0)
                {
                    // make sure we didn't have duplicate flags
                    if (printObjectCode == FALSE) printObjectCode = TRUE;
                    else error(INVALIDINPUTARGUMENTS);
                }
                if (strcmp(argv[i],argV) == 0) 
                {
                    // make sure we didn't have duplicate flags
                    if (printVMExecutionTrace == FALSE) printVMExecutionTrace = TRUE;
                    else error(INVALIDINPUTARGUMENTS);            
                }
            }
            if (halt == TRUE) return;
        }
    }  
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



