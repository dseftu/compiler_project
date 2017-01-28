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

#include <stdio.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

// available instructions
#define LIT 1
#define RTN 2
#define LOD 3
#define STO 4

int main()
{
    // set up the four registers to handle the stack/code
    int BP = 1;
    int SP = 0;
    int PC = 0;
    int IR = 0;

    int code[MAX_CODE_LENGTH];
    int stack[MAX_STACK_HEIGHT];
    return 0;
}

