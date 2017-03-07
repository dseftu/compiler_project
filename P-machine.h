// David M Allen
// P-Machine virtual Machine header
// COP 3402 - Systems Software
#ifndef PMACHINE_H
#define PMACHINE_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define MAX_REGISTERS 16

#define TRUE 1
#define FALSE 0


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

// Handy dandy instruction struct
typedef struct instruction
{
    int op; // opcode
    int r; // reg
    int l; // L
    int m; // M
} instruction;

// set up the four registers to handle the stack/code
int BP = 1;
int SP = 0;
int PC = 0;
instruction IR;

// where to set the activation record lines when printing
// this is purely for displaying stack traces
int AR[MAX_LEXI_LEVELS];
int ARDepth = -1;

// memory stores for code, the stack, and registers
instruction code[MAX_CODE_LENGTH];
int stack[MAX_STACK_HEIGHT];
int R[MAX_REGISTERS];

// reads in input from user
void readInput(char *filename);

// prints the instruction ins.  if newline == TRUE, it prints
// a newline char afterwards.
void printInstruction(instruction ins, int newline);

// converts opcodes to human readable string
char* opcodeToString(int op);

// prints the current stack
void printStack();

// initializes the stack to zeros
void initStack();

// performs the arthimatic/logical operations
void ALU();

// retrieves the next instruction and loads it into the instruction register
void fetch();

// executes the instruction in the instruction register
void execute();

#endif