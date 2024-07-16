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
int convertNum(int num);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;
    int ins = 0, a = 0, b = 0, count = 0;

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
    /* TODO: */
    for (int j = 0; j < NUMREGS; j++)
        state.reg[j] = 0;

    for (state.pc = 0; 1; state.pc++,count++) {
        printState(&state);
        ins = state.mem[state.pc] >> 22 & 0x00000007;
        a = (state.mem[state.pc] >> 19) & 0x00000007;
        b = (state.mem[state.pc] >> 16) & 0x00000007;
        if (ins == 0) {
            state.reg[state.mem[state.pc] & 0x00000007] = state.reg[a] + state.reg[b];
        } else if (ins == 1) {
            state.reg[state.mem[state.pc] & 0x00000007] = ~(state.reg[a] | state.reg[b]);
        } else if (ins == 2) {
            state.reg[b] = state.mem[state.reg[a] + convertNum(state.mem[state.pc] & 0x0000FFFF)];
        } else if (ins == 3) {
            state.mem[state.reg[a] + convertNum(state.mem[state.pc] & 0x0000FFFF)] = state.reg[b];
        } else if (ins == 4 && (state.reg[a] == state.reg[b])) {
                state.pc += convertNum(state.mem[state.pc] & 0x0000FFFF);
                if (state.pc < 0) {
                    printf("error invalid address branch\n");
                    exit(1);
                }
        } else if (ins == 5) {
            state.reg[b] = state.pc + 1;
            state.pc = state.reg[a] - 1;
        } else if (ins == 6) {
            printf("machine halted\n");
            printf("total of %d instructions executed\n",count + 1);
            printf("final state of machine:\n");
            state.pc++;
            break;
        }
    }
    printState(&state);
    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
	/* convert a 16-bit number into a 32-bit Linux integer */
	if (num & (1 << 15)) {
		num -= (1 << 16);
	}
	return (num);
}
