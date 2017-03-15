// David M Allen
// Common
// COP 3402 - Systems Software

#ifndef COMMON_H
#define COMMON_H

    #define TRUE 1
    #define FALSE 0

    #define MAX_IDENTIFIER_LENGTH 11
    #define MAX_NUMBER_LENGTH 5
    #define MAX_SYMBOL_TABLE_SIZE 100

    // Given from the problem statement:
    /*** structure of the symbol table record ***/
    typedef struct
    {
        int kind; 		// const = 1, var = 2, proc = 3.
        char name[MAX_IDENTIFIER_LENGTH];	// name up to 11 chars
        int val; 		// number (ASCII value)
        int level; 		// L  level
        int adr; 		// M  address
    } namerecord_t;

#endif // COMMON_H