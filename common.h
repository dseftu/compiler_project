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
    #define MAX_CODE_LENGTH 500
    #define REG 16

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

    // Basic instructions
    #define LIT 1
    #define RTN 2
    #define LOD 3
    #define STO 4
    #define CAL 5
    #define INC 6
    #define JMP 7
    #define JPC 8

    // techincally these are all "SIO", but I added
    // clarity to the differences.  SIO_O displays
    // output to screen, SIO_I gets input, and
    // SIO_E ends the program
    #define SIO_O 9
    #define SIO_I 10
    #define SIO_E 11

    // Arithmatic and logical instructions
    // these all do a task with 1 or more registers
    // and put the result in a register
    #define NEG 12
    #define ADD 13
    #define SUB 14
    #define MUL 15
    #define DIV 16
    #define ODD 17
    #define MOD 18
    #define EQL 19
    #define NEQ 20
    #define LSS 21
    #define LEQ 22
    #define GTR 23
    #define GEQ 24

    // Given from the problem statement:
    /*** structure of the symbol table record ***/
    typedef struct
    {
        int kind; 		// const = 1, var = 2, proc = 3.
        char name[MAX_IDENTIFIER_LENGTH];	// name up to 11 chars
        int val; 		// number (ASCII value)
        int level; 		// L  level
        int addr; 		// M  address
    } symbol;

    typedef struct
    {
        int kind; 		// const = 1, var = 2, proc = 3.
        char name[MAX_IDENTIFIER_LENGTH];	// name up to 11 chars
        int val; 		// number (ASCII value)
    } lexeme;

    // Handy dandy instruction struct
    typedef struct instruction
    {
        int op; // opcode
        int r; // reg
        int l; // L
        int m; // M
    } instruction;



#endif // COMMON_H
