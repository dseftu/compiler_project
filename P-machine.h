// David M Allen
// P-Machine virtual Machine header
// COP 3402 - Systems Software
#ifndef PMACHINE_H
#define PMACHINE_H


#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define MAX_REGISTERS 16

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

// used to find a variable in a different activation record
int base(int l, int base);

// loads the object code and executes it
void run(char* objectCodeFileName);


#endif