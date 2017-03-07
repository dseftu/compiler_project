// David M Allen
// Compiler General Header
// COP 3402 - Systems Software
#ifndef COMPILER_H
#define COMPILER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#define TRUE 1
#define FALSE 0

// the halt flag.  When set to TRUE, the program ends
int halt = FALSE;

// This loads the object code and executes it
#include "P-machine.h"

// this is the engine which generates the lexeme list
#include "scanner.h"

// this determines if the code is syntactally correct
#include "parser.h"

// this generates the object code for the VM
#include "icg.h"

// error handler which includes all the error codes as well as the messaging
#include "errorHandler.h"


#endif

