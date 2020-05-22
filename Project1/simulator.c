/* LC-2K Instruction-level simulator */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

typedef struct stateStruct {
int pc;
int mem[NUMMEMORY];
int reg[NUMREGS];
int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int);

int main(int argc, char *argv[])
{   
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;
    int i, currInstr, opcode, arg0, arg1, arg2, counter = 0;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }
    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }
    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
            state.numMemory++) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    /* initialize */
    state.pc = 0;
    for (i=0; i<NUMREGS; i++) {
        state.reg[i] = 0;
    }

    /* execute the program */
    while (1) {
        // fetch the instruction to execute.
        currInstr = state.mem[state.pc];
        counter++;

        opcode = currInstr >> 22 & 0b111;
        arg0 = currInstr >> 19 & 0b111; // regA
        arg1 = currInstr >> 16 & 0b111; // regB


        // printState before executing an instruction
        printState(&state);

        switch(opcode){
        // add
        case 0:
            arg2 = currInstr & 0b111;
            state.reg[arg2] = state.reg[arg0] + state.reg[arg1];
            break;
        // nor
        case 1:
            arg2 = currInstr & 0b111;
            state.reg[arg2] = ~(state.reg[arg0] | state.reg[arg1]);
            break;
        // lw
        case 2:
            arg2 = convertNum(currInstr & 0xffff);
            state.reg[arg1] = state.mem[arg2 + state.reg[arg0]];
            break;
        // sw
        case 3:
            arg2 = convertNum(currInstr & 0xffff);
            state.mem[arg2 + state.reg[arg0]] = state.reg[arg1];
            break;
        // beq
        case 4:
            arg2 = convertNum(currInstr & 0xffff);
            if (state.reg[arg0] == state.reg[arg1])
                state.pc += arg2;
            break;
        // jalr
        case 5:
            state.reg[arg1] = state.pc + 1;
            state.pc = state.reg[arg0] - 1;
            break;
        // halt
        case 6:
            goto HALT;
        // noop
        case 7:
            break;
        }

        state.pc++;
    }
HALT:
    state.pc++;

    printf("machine halted\ntotal of %d instructions executed\nfinal state of machine:\n", counter);

    printState(&state);

    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit Linux integer */
    if (num & (1<<15) ) {
        num -= (1<<16);
    }
    return(num); 
}