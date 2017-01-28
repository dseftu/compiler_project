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

int main()
{
    return 0;
}

void readInput(string filename)
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

