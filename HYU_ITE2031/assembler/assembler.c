/* Assembler code fragment for LC-2K */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000
#define MAXLABEL 1000
int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int check(char* reg);
struct labels {
    char label[MAXLINELENGTH];
    int lcount;
};

int main(int argc, char *argv[]) 
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], 
			 arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
    struct labels labels[MAXLABEL];
    int lcount = 0, count = 0, flag = 0;
    char type;
    int ins = 0, result = 0, a = 0, b = 0, c = 0;

	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
				argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	/* here is an example for how to use readAndParse to read a line from
		 inFilePtr */
	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		if (strlen(label) > 6){
            printf("error in label length\n");
            exit(1);
        } else if (isNumber(&label[0])) {
            printf("error in label start number\n");
            exit(1);
        } 
        if (strlen(label)) {
            for(int i = 0; i < lcount; i++) {
                if (!strcmp(labels[i].label, label)){
                    printf("error in label duplication\n");
                    exit(1);
                }
            }
            strcpy(labels[lcount].label, label);
            labels[lcount++].lcount = count;
        }
        count++;
	}

	/* TODO: Phase-1 label calculation */

	/* this is how to rewind the file ptr so that you start reading from the
		 beginning of the file */
	rewind(inFilePtr);
    count = 0;
	/* TODO: Phase-2 generate machine codes to outfile */
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        if (!strcmp(opcode, "add")) {
            type = 'R';
            ins = 0;
        } else if (!strcmp(opcode, "nor")){
            type = 'R';
            ins = 1;
        } else if (!strcmp(opcode, "lw")){
            type = 'I';
            ins = 2;
        } else if (!strcmp(opcode, "sw")){
            type = 'I';
            ins = 3;
        } else if (!strcmp(opcode, "beq")){
            type = 'I';
            ins = 4;
        } else if (!strcmp(opcode, "jalr")){
            type = 'J';
            ins = 5;
        } else if (!strcmp(opcode, "halt")){
            type = 'O';
            ins = 6;
        } else if (!strcmp(opcode, "noop")){
            type = 'O';
            ins = 7;
        } else if (!strcmp(opcode, ".fill")) {
            type = 'O';
            ins = 0;
        } else {
            printf("error invalid opcode\n");
            exit(1);
        }
        result = ins << 22;
        if (type == 'O') {
            if (ins == 0 && !check(arg0)){
                for (int i = 0; i < lcount; i++) {
                    if (!strcmp(arg0, labels[i].label)) {
                        c = labels[i].lcount;
                        flag = 1;
                    }
                }
                if (flag == 0) {
                    printf("error wrong register\n");
                    exit(1);
                }
                flag = 0;
                result |= c;
            }
            else if (ins == 0 && check(arg0)) {
                c = atoi(arg0);
                if (c > 2147483647 || c < -2147483648) {
                    printf("error wrong range of numeric value\n");
                    exit(1);
                }
                result |= c;
            }
        }
        else 
        {
            if (!check(arg0) || !check(arg1)) {
                printf("error wrong register\n");
                exit(1);
            }
            a = atoi(arg0);
            b = atoi(arg1);
            if (type == 'R') {
                if (!check(arg2)) {
                    printf("error wrong register\n");
                    exit(1);
                }
                c = atoi(arg2);
            } else if (type == 'I') {
                if (isNumber(arg2)) {
                    c = atoi(arg2);
                    if (c > 32767 || c < -32768) {
                        printf("error wrong range offsetField\n");
                        exit(1);
                    }
                }
                else {
                    for(int i = 0; i < lcount; i++) {
                        if (!strcmp(arg2, labels[i].label)) {
                            flag = 1;
                            c = labels[i].lcount;
                        }
                    }
                    if (flag == 0) {
                        printf("error wrong label\n");
                        exit(1);
                    }
                    flag = 0;
                    if (ins == 4) c = c - (count + 1);
                }
                c = c & 0x0000FFFF;
            } else if (type == 'J') c = 0;
            result |= c;
            result |= a << 19;
            result |= b << 16;
        }
        count++;
        fprintf(outFilePtr, "%d\n", result);
    }
	/* after doing a readAndParse, you may want to do the following to test the
		 opcode */
	if (inFilePtr) {
		fclose(inFilePtr);
	}
	if (outFilePtr) {
		fclose(outFilePtr);
	}
	return(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
		char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* reached end of file */
		return(0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL) {
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label)) {
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%"
			"[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
	return(1);
}

int isNumber(char *string)
{
	/* return 1 if string is a number */
	int i;
	return( (sscanf(string, "%d", &i)) == 1);
}

int check(char *reg)
{
    if (!isNumber(reg))
        return 0;
    int a = atoi(reg);
    if (a > 7 && a < 0)
        return 0;
    return 1;
}
