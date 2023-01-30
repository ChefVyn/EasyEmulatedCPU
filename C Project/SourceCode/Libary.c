#include "HeaderFiles/Emulator.h"

void allocation()
{
    Register = malloc(NumberReg * sizeof(uint8_t)); // Dynamic Memory Allocation (Register)
    Data = malloc(NumberData * sizeof(uint8_t));    // Dynamic Memory Allocation (Data)
    if ((Register == NULL) || (Data == NULL))
    { // Checking if allocation worked
        printf("Can not allocate memory!");
        exit(2); // Error code 2 if not possible to allocate memory
    }
}

_Bool lineRead(FILE *Pr, char *Op, char *First, char *Sec)
{ // Function that reads the Assambler code
    uint8_t input_1 = fscanf(Pr, "%s", Op);
    if (Op[0] == 'N')
    {
        if (input_1 == 1)
        {
            First[0] = '\0';
            Sec[0] = '\0';
            return 1;
        }
    }

    uint8_t input_2 = fscanf(Pr, "%s"
                                 "%s",
                             First, Sec);
    if ((input_1 + input_2) == 3)
        return 1;
    else
        return 0;
}

void printRegDat(uint8_t *Reg, uint8_t *Dat)
{
    for (uint8_t i = 0; i < NumberReg; i++)
    { // Output of all Registers to show the Operations
        printf("R%u:%u  ", i, Reg[i]);
    }
    printf("\n");
    for (uint8_t i = 0; i < NumberData; i++)
    { // Output of all Data to show the Operations
        printf("D%u:%u  ", i, Dat[i]);
    }
}

void DataFilePrint(uint8_t r[16], FILE *fp)
{
    if (fp == NULL)
    {
        printf("\nError opening file!\n");
        exit(1);                                    // 1 if something went wrong when creating the file
    }

    for (uint8_t i = 0; i < 16; i++)
    {
        char hex_string[3];
                                                    // convert decimal value of r[i] to hexadecimal string
        sprintf(hex_string, "%02x", r[i]);         // 02 before is a width specifier that means the output should be at least 2 characters wide
        fprintf(fp, "D%d: 0x%s  ", i, hex_string); // 0x is added before the hex_string -> following value is in hexadecimal format
    }
    fprintf(fp,"\n");
}


uint8_t firstReInBinary(char *number)
{ // convert to Binary (Operant 1: Rn)
    uint8_t dezVal;
    if (number[0] == 'R')
    {
        if (strlen(number) == 3)
        {
            dezVal = number[1] - 48;
        }
        else
        {
            dezVal = number[2] - 38;
        }
    }
    else
    {
        if (strlen(number) == 4)
        {
            dezVal = number[2] - 48;
        }
        else
        {
            dezVal = number[3] - 38;
        }
    }
    return dezVal;
}

uint8_t secReInBinary(char *number)
{ // convert to Binary (Operant 2: Rm)
    uint8_t dezVal;
    if ((number[0] == 'R') || (number[0] == '#'))
    {
        if (strlen(number) == 2)
        {
            dezVal = number[1] - 48;
        }
        else
        {
            dezVal = number[2] - 38;
        }
    }
    else
    {
        if (strlen(number) == 3)
        {
            dezVal = number[2] - 48;
        }
        else
        {
            dezVal = number[3] - 38;
        }
    }
    return dezVal;
}

uint8_t andAssembly(uint8_t m, uint8_t n)
{                          // based on registers value
    uint8_t binaryNum[32]; // contains of binary result unsigned int = 2^32-1
    uint8_t i = 0;
    uint8_t result = 0; // contains of decimal result
    while (m > 0 || n > 0)
    {                                      // only stop when all binar reach zero
        binaryNum[i] = (m % 2) && (n % 2); // get binary for each element also compare AND

        m = m > 0 ? m / 2 : 0; // if one larger than other, use zero as digit
        n = n > 0 ? n / 2 : 0;
        i++;
    }

    // back to decimal from binary

    for (uint8_t j = 0; j < i; j++)
    {
        result += (uint8_t)(pow(2, j)) * binaryNum[j]; // no need to reverse since the last will be biggest
    }
    return result;
}

uint8_t orAssembly(uint8_t m, uint8_t n)
{
    uint8_t binaryNum[32]; // contains of binary result unsigned int = 2^32-1
    uint8_t i = 0;
    uint8_t result = 0; // contains of decimal result
    while (m > 0 || n > 0)
    {                                      // only stop when all binar reach zero
        binaryNum[i] = (m % 2) || (n % 2); // get binary for each element also compare AND

        m = m > 0 ? m / 2 : 0; // if one larger than other, use zero as digit
        n = n > 0 ? n / 2 : 0;
        i++;
    }

    // back to decimal from binary
    i = i - 1;
    for (int j = 0; j <= i; j++)
        result += (int)(pow(2, j)) * binaryNum[j];
    return result;
}

void zeroit(uint8_t *Array, uint8_t quantity)
{
    for (uint8_t i = 0; i < quantity; i++)
    {
        Array[i] = 0;
    }
}

void funcCall(const uint8_t *Opcode, const uint8_t *Operand_1, const uint8_t *Operand_2, uint8_t *Reg, uint8_t *Dat, uint8_t *PC)
{ // enum funktion für den Funktioncall
    enum Operations
    {
        MOV_RN_addr,
        MOV_addr_Rn,
        MOV_Rn_aRm,
        MOV_aRn_Rm,
        MOV_Rn_literal,
        MOV_Rn_Rm,
        ADD_Rn_Rm,
        SUB_Rn_Rm,
        MUL_Rn_Rm,
        DIV_Rn_Rm,
        AND_Rn_Rm,
        OR_Rn_Rm,
        JZ_Rn_Rm,
        CMP_Rn_Rm,
        LESS_Rn_Rm,
        NOP
    };
    switch (*Opcode)
    {
    case MOV_RN_addr:
        Reg[*Operand_1] = Dat[*Operand_2];
        // printf("%hhu \n", *Operand_1);
        break;
    case MOV_addr_Rn:
        Dat[*Operand_1] = Reg[*Operand_2];
        break;
    case MOV_Rn_aRm:
        // Data [ Deference the Reg address (Dereference the Operand_2)]
        Reg[*Operand_1] = Dat[*&Reg[*Operand_2]];
        break;
    case MOV_aRn_Rm:
        Dat[*&Reg[*Operand_1]] = Reg[*Operand_2];
        break;
    case MOV_Rn_literal:
        Reg[*Operand_1] = *Operand_2;
        break;
    case MOV_Rn_Rm:
        Reg[*Operand_1] = Reg[*Operand_2];
        break;
    case ADD_Rn_Rm:
        Reg[*Operand_1] = Reg[*Operand_1] + Reg[*Operand_2];
        break;
    case SUB_Rn_Rm:
        Reg[*Operand_1] = Reg[*Operand_1] + (256 - Reg[*Operand_2]);
        break;
    case MUL_Rn_Rm:
        Reg[*Operand_1] = Reg[*Operand_1] * Reg[*Operand_2];
        break;
    case DIV_Rn_Rm:
        if (Reg[*Operand_2] != 0)
        {
            Reg[*Operand_1] = Reg[*Operand_1] / Reg[*Operand_2];
        }
        else
        {
            printf("\nCannot divide by zero!");
        }
        break;
    case AND_Rn_Rm:
        Reg[*Operand_1] = andAssembly(Reg[*Operand_1], Reg[*Operand_2]);
        break;
    case OR_Rn_Rm:
        Reg[*Operand_1] = orAssembly(Reg[*Operand_1], Reg[*Operand_2]);
        break;
    case JZ_Rn_Rm:
        if (Reg[*Operand_1] == 0)
        {
            int rel = *Operand_2;
            *PC = *PC + rel - 1;
        }
        break;
    case CMP_Rn_Rm:
        if (Reg[*Operand_1] == Reg[*Operand_2])
        {
            Reg[*Operand_1] = Reg[*Operand_2];
        }
        break;
    case LESS_Rn_Rm:
        if (Reg[*Operand_1] < Reg[*Operand_2])
        {
            Reg[*Operand_1] = Reg[*Operand_2];
        }
        break;
    case NOP:
        break;
    default:
        printf("Something went wrong!");
    }
}

_Bool whichMOV(char *First, char *Sec, uint8_t *Opcode, uint8_t *Operand_1, uint8_t *Operand_2)
{

    *Operand_1 = firstReInBinary(First); // giving Operand_1 a value before the switch case to make it more readable
    *Operand_2 = secReInBinary(Sec);     // giving Operand_2 a value before the switch case to make it more readable

    if (First[0] == 'R')
    {
        switch (Sec[0])
        {
        case '@': // case für MOV RN, @Rm
            *Opcode = 0b0010;
            break;
        case '#': // case für MOV RN, #literal
            *Opcode = 0b0100;
            break;
        case 'R': // case für MOV RN, Rm
            *Opcode = 0b0101;
            break;
        default: // case für MOV RN, addr
            *Opcode = 0b0000;
            *Operand_2 = atoi(Sec);
            break;
        }
    }
    else if (First[0] == '@')
    { // case für MOV @Rn, Rm
        *Opcode = 0b0011;
    }
    else
    { // case für MOV addr, Rn
        *Opcode = 0b0001;
        *Operand_1 = atoi(First);
    }
    return 1;
}

_Bool assamblerToOp(const char *Op, char *First, char *Sec, uint8_t *Opcode, uint8_t *Operand_1, uint8_t *Operand_2)
{
    enum Opcodes
    {
        MOV_MUL = 'M',
        ADD_AND = 'A',
        SUB = 'S',
        DIV = 'D',
        OR = 'O',
        JZ = 'J',
        CMP = 'C',
        LESS = 'L',
        NOP = 'N'
    }; // Assigning a char to each Operation to check the case in the switch statement

    *Operand_1 = firstReInBinary(First); // giving Operand_1 a value before the switch case to make it more readable
    *Operand_2 = secReInBinary(Sec);     // giving Operand_2 a value before the switch case to make it more readable

    switch (Op[0])
    { // Finding the opcode with the first letter of the opcode
    case MOV_MUL:
        if (Op[1] == 'O')
            return whichMOV(First, Sec, Opcode, Operand_1, Operand_2); // case für MOV
        if (Op[1] == 'U')
            *Opcode = 0b1000; // case für MUL Rn, Rm
        return 1;
    case ADD_AND:
        if (Op[1] == 'D')
            *Opcode = 0b0110; // case für ADD Rn, Rm
        if (Op[1] == 'N')
            *Opcode = 0b1010; // case für AND Rn, Rm
        return 1;
    case SUB:
        *Opcode = 0b0111; // case für SUB
        return 1;
    case DIV: // case für DIV
        *Opcode = 0b1001;
        return 1;
    case OR: // case für OR
        *Opcode = 0b1011;
        return 1;
    case JZ: // case für JZ
        *Opcode = 0b1100;
        *Operand_2 = atoi(Sec);
        return 1;
    case CMP: // case für CMP
        *Opcode = 0b1101;
        return 1;
    case LESS: // case für LESS
        *Opcode = 0b1110;
        return 1;
    case NOP: // case für NOP
        *Opcode = 0b1111;
        return 1;
    default:
        return 0;
    }
}

void Output(uint8_t r[16], const char *filename)
{

    FILE *fp;
    size_t len = strlen(filename);
    const char *extension = &filename[len - 4];

    if (strcmp(extension, ".hex") == 0)                                 // writing in a .hex
    {
        fp = fopen(filename, "w");
        if (fp == NULL)
        {
            printf("\nError opening file!\n");
            exit(1);                                                    // 1 if something went wrong when creating the file
        }
        for (uint8_t i = 0; i < 16; i++)
        {
            char hex_string[3];
            // convert decimal value of r[i] to hexadecimal string
            sprintf(hex_string, "%02x", r[i]);                          // 02 before is a width specifier that means the output should be at least 2 characters wide
            fprintf(fp, "R%d: 0x%s  ", i, hex_string);                  // 0x is added before the hex_string -> following value is in hexadecimal format
        }
    } else                                                              // if the exention is not .hex or .txt it will remove the created file and return an Error message
    {
        printf("Error: Wrong file extension!\n");
        exit(1);                                                        // 1 if something went wrong when creating the file
    }
    fclose(fp);
}
