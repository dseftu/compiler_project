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
//
// In addition, there will be 16 registers for the
// program to store and write data to.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "common.h"
#include "P-machine.h"

extern int halt;
extern int printObjectCode;
extern int printVMExecutionTrace;

void run(char *objectCodeFileName)
{
    // read in the input file
    readInput(objectCodeFileName);

    // init the instruction register
    IR.op = 0;
    IR.r = 0;
    IR.l = 0;
    IR.m = 0;

    // init the activation record lines
    // these will be used to check to see if we should print a line or not when
    // printing the stack
    int i = 0;
    for (i = 0; i < MAX_LEXI_LEVELS; i++)
        AR[i] = -1;

    // init the stack to all zeros
    initStack();

    if (printVMExecutionTrace == TRUE) printf("\nInitial Values\t\t\t\tPC\tBP\tSP\n");

    // start the main execution loop
    while (halt == FALSE && BP != 0)
    {
        // get the next instruction
        fetch();

        // check for halted condition in case fetch failed
        if (halt == TRUE) break;

        // save the PC to print after execution
        int tempPC = PC;

        // execute the next instruction
        execute();

        // print the current stack, including instruction register
        if (printVMExecutionTrace == TRUE)
        {
            printf("%d\t", tempPC);
            printStack();
        }
        
    }

}

void readInput(char *filename)
{
    // opens the file
    FILE *fid;

    fid = fopen(filename, "r");

    if (fid == NULL)
    {
        // can't open file
        printf("Unable to open file!");
        halt = TRUE;
    }
    else
    {
        int i = 0;
        int opcode, r, l, m;

        if (printObjectCode == TRUE) printf("Line\tOP\tR\tL\tM\n");

        while (i < MAX_CODE_LENGTH && fscanf(fid, "%d %d %d %d", &opcode, &r, &l, &m) == 4)
        {
            code[i].op = opcode;
            code[i].r = r;
            code[i].l = l;
            code[i].m = m;
            if (printObjectCode == TRUE)
            {
                printf("%d\t",i);
                printInstruction(code[i],TRUE);
            }
            // advance the counter
            i++;

        }

        // clear rest of code
        while (i < MAX_CODE_LENGTH)
        {
            // setting any other instructions past the end of the code file to
            // halt statements.
            code[i].op = 11;
            code[i].r = 0;
            code[i].l = 0;
            code[i].m = 2;

            // advance the counter
            i++;
        }

        fclose(fid);

        if (printObjectCode == TRUE) printf("\n");
    }
}

// prints the instruction using the following format:
// Line OP  R   L   M
// eg.
// 13   sto 0   0   5
// optional int newline dictates whether or not to
// print a newline char or not.
void printInstruction(instruction ins, int newline)
{
    printf("%s\t%d\t%d\t%d", opcodeToString(ins.op), ins.r, ins.l, ins.m);

    if (newline == TRUE) printf("\n");
}

char* opcodeToString(int op)
{
    switch (op)
    {
        case LIT:
            return "lit";
        case RTN:
            return "rtn";
        case LOD:
            return "lod";
        case STO:
            return "sto";
        case CAL:
            return "cal";
        case INC:
            return "inc";
        case JMP:
            return "jmp";
        case JPC:
            return "jpc";
        case SIO_O:
            return "sio";
        case SIO_I:
            return "sio";
        case SIO_E:
            return "sio";
        case NEG:
            return "neg";
        case ADD:
            return "add";
        case SUB:
            return "sub";
        case MUL:
            return "mul";
        case DIV:
            return "div";
        case ODD:
            return "odd";
        case MOD:
            return "mod";
        case EQL:
            return "eql";
        case NEQ:
            return "neq";
        case LSS:
            return "lss";
        case LEQ:
            return "leq";
        case GTR:
            return "gtr";
        case GEQ:
            return "geq";
        default:
            printf("unexpected opcode!");
            halt = TRUE;
            return "";
    }
    return "";
}

// prints the current program data
void printStack()
{
    // print the instruction register
    printInstruction(IR, FALSE);

    // print the PC, BP, and SP
    printf("\t%d\t%d\t%d", PC, BP, SP);

    // iterate through the stack up to SP
    int i, c;
    for (i = 0; i <= SP; i++)
    {
        // see if we need to print a break
        for (c = 0; c <= ARDepth; c++)
            if (AR[c] == i-1) printf("\t|");

        printf("\t%d", stack[i]);
    }

    printf("\n");

}

// initializes the stack to all zeros
void initStack()
{
    int i;
    for (i = 0; i < MAX_STACK_HEIGHT; i++)
        stack[i] = 0;

}

// performs artithmatic and logical instrutions
void ALU()
{
    switch (IR.op)
    {
        // i == R, j == L, k == M
        case NEG:
            R[IR.r] = -R[IR.r];
            break;
        case ADD:
            R[IR.r] = R[IR.l] + R[IR.m];
            break;
        case SUB:
            R[IR.r] = R[IR.l] - R[IR.m];
            break;
        case MUL:
            R[IR.r] = R[IR.l] * R[IR.m];
            break;
        case DIV:
            R[IR.r] = R[IR.l] / R[IR.m];
            break;
        case ODD:
            R[IR.r] = R[IR.l] % 2;
            break;
        case MOD:
            R[IR.r] = R[IR.l] % R[IR.m];
            break;
        case EQL:
            R[IR.r] = R[IR.l] == R[IR.m];
            break;
        case NEQ:
            R[IR.r] = R[IR.l] != R[IR.m];
            break;
        case LSS:
            R[IR.r] = R[IR.l] < R[IR.m];
            break;
        case LEQ:
            R[IR.r] = R[IR.l] <= R[IR.m];
            break;
        case GTR:
            R[IR.r] = R[IR.l] > R[IR.m];
            break;
        case GEQ:
            R[IR.r] = R[IR.l] >= R[IR.m];
            break;
        default:
            printf("unexpected opcode of %d!", IR.op);
            halt = TRUE;
            return;
    }
}

// fetchs the next instruction and places
// it into the instruction register
// increments program counter by 1
void fetch()
{
    IR = code[PC++];
}

// executes the instruciton that is in
// in instruction register
void execute()
{
    switch (IR.op)
    {
        case LIT: // load immiediate into register
            R[IR.r] = IR.m;
            break;
        case RTN: // returns from calling method
            AR[ARDepth] = -1;
            ARDepth--;
            SP = BP -1;
            BP = stack[SP+3];
            PC = stack[SP+4];
            break;
        case LOD: // loads into register from stack
            R[IR.r] = stack[base(IR.l, BP) + IR.m];
            break;
        case STO: // stores from register into stack
            stack[base(IR.l,BP) + IR.m] = R[IR.r];
            break;
        case CAL:   // calls procedure
            ARDepth++;
            AR[ARDepth] = SP;
            stack[SP+1] = 0;
            stack[SP+2] = base(IR.l, BP);
            stack[SP+3] = BP;
            stack[SP+4] = PC;
            BP = SP + 1;
            PC = IR.m;
            break;
        case INC:   // allocates M locals
            SP = SP + IR.m;
            break;
        case JMP:   // jump to instruction
            PC = IR.m;
            break;
        case JPC:   // conditional jump
            if (R[IR.r] == 0)
                PC = IR.m;
            break;
        case SIO_O: // writes the register to the screen
            printf("%d\n", R[IR.r]);
            break;
        case SIO_I: // reads input to register
            scanf("%d", &R[IR.r]);
            printf("\n");
            break;
        case SIO_E: // End of program
            halt = TRUE;
            break;
        default: // any other opcode is an ALU code
            ALU();
    }

}

// Code provided by the problem statement:
// Find base L levels down
int base(int l, int base)
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

