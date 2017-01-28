// David M Allen
// P-Machine virtual Machine
// COP 3402 - Systems Software
// This program will emulate a stack machine with two memory
// stores:
// The "stack" which will be organized as a stack and will contain
// the data to be used by the PM/0 CPU.
// The "code", which will be a list and contain the instructions for the VM.
//
// There will be four registers to handle the stack and code segments:
// BP, the base pointer
// SP, the stack pointer
// PC, the program counter
// IR, the instruciton register

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define MAX_REGISTERS 16

// available instructions
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

#define HALTED -1

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
int IR = 0;

int instruction[MAX_CODE_LENGTH];
int stack[MAX_STACK_HEIGHT];
int registers[MAX_REGISTERS];

int halt = 0;

int main()
{
    
    return 0;
}

void readInput(string filename)
{
    // opens the file

    // while data still exists, continue to read

    // reads in 4 ints (which contitutes an instruction)

    // loads into next instruction

    // fill the rest of the instruction array will blanks    
}

// prints the instruction using the following format:
// Line OP  R   L   M
// eg.
// 13   sto 0   0   5
// optional bool newline dictates whether or not to
// print a newline char or not.
void printInstruction(instruction ins, bool newline)
{

}

// prints out the program in intrepreted assembly language with line numbers
void printCode()
{

}

// prints the current program data
void printStack()
{

}

// initializes the stack to all zeros
void initStack()
{
    
}

// performs artithmatic and logical instrutions
void ALU(instruction ins)
{

}

// fetchs the next instruction and places
// it into the instruction register
// increments program counter by 1
void fetch()
{

}

// executes the instruciton that is in
// in instruction register
void execute()
{
    
}

// Code provided by the problem statement:
// Find base L levels down
int base(l, base)
{
    int b1;
    b1 = base;

    while (l > 0)
    {
        b1 = stack[b1 +1];
        l--;
    }
    return b1;
}

