/* Assembler code for LC-2K */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000
#define MAXLABELLENGTH 7

typedef struct _list {
    char label[MAXLABELLENGTH];
    int addr;
    struct _list *next;
} list;

list* initList(void);
void insert(list*, char*, int);
void freeList(list*);
int findAddr(list*, char*);
void readLabels(FILE *, list *);
int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int encodeRType(int, int, int);
int encodeIType(int, int, int);
int encodeJType(int, int);

int main(int argc, char *argv[])
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], 
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
    list *labelList;
    int resultMc = 0, argint0, argint1, argint2, counter = 0;
    
    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n", argv[0]);
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

    // create label list
    labelList = initList();

    /* Pass 1 : calculate the address for every symbolic label */
    readLabels(inFilePtr, labelList);


    /* Pass 2 : generate a machine-language instruction */
    
    // At first, rewinding the input file
    rewind(inFilePtr);

    while ( readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
        argint0 = atoi(arg0);
        argint1 = atoi(arg1);
        argint2 = atoi(arg2);

        if (!strcmp(opcode, "add")) {
            resultMc = 0b000 | encodeRType(argint0, argint1, argint2);
        } else if (!strcmp(opcode, "nor")) {
            resultMc = 0b001 << 22 | encodeRType(argint0, argint1, argint2);
        } else if (!strcmp(opcode, "lw")) {
            if (isNumber(arg2)) {
                if (argint2 < -32768 || argint2 > 32767) {
                    printf("error: offsetFields that don't fit in 16 bits\n%d\n", argint2);
                    exit(1);
                }
                resultMc = 0b010 << 22 | encodeIType(argint0, argint1, argint2);
            } else {
                if (strlen(arg2) > MAXLABELLENGTH - 1) {
                    printf("error: invalid symbolic address\n%s\n", arg2);
                    exit(1);
                }
                argint2 = findAddr(labelList, arg2);
                // address of label is always fit in 16-bits.
                // because the memory size is 65536 word.
                resultMc = 0b010 << 22 | encodeIType(argint0, argint1, argint2);
            }
        } else if (!strcmp(opcode, "sw")) {
            if (isNumber(arg2)) {
                if (argint2 < -32768 || argint2 > 32767) {
                    printf("error: offsetFields that don't fit in 16 bits\n%d\n", argint2);
                    exit(1);
                }
                resultMc = 0b011 << 22 | encodeIType(argint0, argint1, argint2);
            } else {
                if (strlen(arg2) > MAXLABELLENGTH - 1) {
                    printf("error: invalid symbolic address\n%s\n", arg2);
                    exit(1);
                }
                argint2 = findAddr(labelList, arg2);
                // address of label is always fit in 16-bits.
                // because the memory size is 65536 word.
                resultMc = 0b011 << 22 | encodeIType(argint0, argint1, argint2);
            }
        } else if (!strcmp(opcode, "beq")) {
            if (isNumber(arg2)) {
                if (argint2 < -32768 || argint2 > 32767) {
                    printf("error: offsetFields that don't fit in 16 bits\n%d\n", argint2);
                    exit(1);
                }
                resultMc = 0b100 << 22 | encodeIType(argint0, argint1, argint2);
            } else {
                if (strlen(arg2) > MAXLABELLENGTH - 1) {
                    printf("error: invalid symbolic address\n%s\n", arg2);
                    exit(1);
                }
                argint2 = findAddr(labelList, arg2) - 1 - counter;
                resultMc = 0b100 << 22 | encodeIType(argint0, argint1, argint2);
            }
        } else if (!strcmp(opcode, "jalr")) {
            resultMc = 0b101 << 22 | encodeJType(argint0, argint1);
        } else if (!strcmp(opcode, "halt")) {
            resultMc = 0b110 << 22;
        } else if (!strcmp(opcode, "noop")) {
            resultMc = 0b111 << 22;
        } else if (!strcmp(opcode, ".fill")) {
            if (isNumber(arg0)) {
                resultMc = argint0;
            } else {
                resultMc = findAddr(labelList, arg0);
            }
        } else {
            printf("error: unrecognized opcode\n%-7s\n", opcode);
            exit(1);
        }

        // fprintf(outFilePtr, "%x\n", resultMc);
        fprintf(outFilePtr, "%d\n", resultMc);

        counter++;
    }

    fclose(inFilePtr);
    fclose(outFilePtr);

    freeList(labelList);

    exit(0);
}


/*
 * Read and parse a line of the assembly-language file. Fields are returned 
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *   0 if reached end of file
 *   1 if all went well
 *
 * Because readLabels function already handle the exception,
 * this function cannot handle them.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0, char *arg1, char *arg2)
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

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n\r ]", label)) {
        /* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
    * Parse the rest of the line. Would be nice to have real regular
    * expressions, but scanf will suffice.
    */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
            opcode, arg0, arg1, arg2);
    return(1);
}

int isNumber(char *string)
{
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}

/*
 * return initial empty head of list
 */
list* initList(void)
{
    list* head = malloc(sizeof(list));

    head->addr = 0;
    head->label[0] = '\0';
    head->next = NULL;

    return head;
}

/*
 * Insert the label node.
 * If duplicate, insertion fail.
 * Head must not be NULL.
 * Length of label excluding ending null character 
 *   should be less than MAXLABELLENGTH.
 * 
 * exit(1) if label is duplicate.
 */
void insert(list* head, char* label, int addr)
{
    list* node = head->next;
    while(node && strncmp(node->label, label, MAXLABELLENGTH)) {
        node = node->next;
    }
    if(node) {
        printf("error: duplicate labels\n%-7s\n", label);
        exit(1);
    }

    node = malloc(sizeof(list));
    node->addr = addr;
    strncpy(node->label, label, MAXLABELLENGTH);
    node->next = head->next;
    head->next = node;
}

void freeList(list* head)
{
    list *temp;
    while(head) {
        temp = head;
        head = head->next;
// printf("list addr:%d, label:%-7s\n", temp->addr, temp->label);
        free(temp);
    }
}

/*
 * Find the address corresponding to given label.
 * Head must not be NULL.
 * 
 * exit(1) if label is not in list.
 */
int findAddr(list* head, char* label)
{
    list* node = head->next;
    while(node && strncmp(node->label, label, MAXLABELLENGTH)) {
        node = node->next;
    }
    if(!node) {
        printf("error: undefined label\n%-7s\n", label);
        exit(1);
    }

    return node->addr;
}

/*
 * Read and insert labels to list.
 *
 * exit(1) if duplicate label is detected,
 *            line is too long, 
 *            or label is invalid.
 */
void readLabels(FILE *inFilePtr, list* head)
{
    char line[MAXLINELENGTH];
    char label[MAXLINELENGTH];
    int addr = 0;

    /* read the line from the assembly-language file */
    while(fgets(line, MAXLINELENGTH, inFilePtr) != NULL) {
        /* initialize label */
        label[0] = '\0';
        
        /* check for line too long (by looking for a \n) */
        if (strchr(line, '\n') == NULL) {
            /* line too long */
            printf("error: line too long\n");
            exit(1);
        }

        /* is there a label? */
        if (sscanf(line, "%[^\t\n\r ]", label)) {
            /* successfully read label */

            /* is label too long? */
            if (strlen(label) > MAXLABELLENGTH - 1 || (label[0] >= '0' && label[0] <= '9')) {
                printf("error: invalid label\n%s\n", label);
                exit(1);
            }
            insert(head, label, addr);
        }

        addr++;
    }

    /* reached end of file */
}

/*
 * Generate machine code of R-type instruction.
 * This function generate machine code without opcode part.
 */
int encodeRType(int regA, int regB, int destReg)
{
    return (regA & 0b111) << 19 | (regB & 0b111) << 16 | (destReg & 0b111);
}

/*
 * Generate machine code of I-type instruction.
 * This function generate machine code without opcode part.
 */
int encodeIType(int regA, int regB, int offsetField)
{
    return (regA & 0b111) << 19 | (regB & 0b111) << 16 | (offsetField & 0xffff);
}

/*
 * Generate machine code of J-type instruction.
 * This function generate machine code without opcode part.
 */
int encodeJType(int regA, int regB)
{
    return (regA & 0b111) << 19 | (regB & 0b111) << 16;
}