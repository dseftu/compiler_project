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

    #define nulsym 1
    #define identsym 2
    #define numbersym 3
    #define plussym 4
    #define minussym 5
    #define multsym 6
    #define slashsym 7
    #define oddsym 8
    #define eqlsym 9
    #define neqsym 10
    #define lessym 11
    #define leqsym 12
    #define gtrsym 13
    #define geqsym 14
    #define lparentsym 15
    #define rparentsym 16
    #define commasym 17
    #define semicolonsym 18
    #define periodsym 19
    #define becomessym 20
    #define beginsym 21
    #define endsym 22
    #define ifsym 23
    #define thensym 24
    #define whilesym 25
    #define dosym 26
    #define callsym 27
    #define constsym 28
    #define varsym 29
    #define procsym 30
    #define writesym 31
    #define readsym 32
    #define elsesym 33

    // Given from the problem statement:
    /*** structure of the symbol table record ***/
    typedef struct
    {
        int kind; 		// const = 1, var = 2, proc = 3.
        char name[MAX_IDENTIFIER_LENGTH];	// name up to 11 chars
        int val; 		// number (ASCII value)
        int level; 		// L  level
        int adr; 		// M  address
    } symbol;

    typedef struct
    {
        int kind; 		// const = 1, var = 2, proc = 3.
        char name[MAX_IDENTIFIER_LENGTH];	// name up to 11 chars
        int val; 		// number (ASCII value)
    } lexeme;

    


#endif // COMMON_H