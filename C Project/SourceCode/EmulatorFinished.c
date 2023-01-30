#include "HeaderFiles/Emulator.h"

const char *HexFile = "RegDataOutput/Register.hex";
const char *TextFile = "RegDataOutput/Data.txt";

int main(int argn, char *argv[])
{
    FILE *Program = fopen(argv[1], "r"); // Checking if the File is there and readable
    FILE *fpData = fopen("RegDataOutput/Data.txt", "w");
    if (Program == NULL)
    {
        printf("Can not open file! %s\n", argv[1]);
        return 1; // Error code 1 if not possible to open File
    }

    allocation();

    printf("Program name: %s\n", argv[1]); // Programmname
    uint8_t PC = 0;                        // Initiallisierung
    _Bool reading, convertToBinary;
    uint8_t Opcode, Operand_1, Operand_2;
    char Operator[6] = "START", First[6] = "", Second[5] = "";
    uint8_t count = 0;

    do
    {
        if (Operator[1] != 'T')
        {
            funcCall(&Opcode, &Operand_1, &Operand_2, Register, Data, &PC);                 // Calling the Function which executes the operations
            printf("\n\nPC: %u  %s %s %s\n", PC++, Operator, First, Second);
            if (First[0] == 'R' || First[1] == 'R')
            { // looking if First(Rn) a register --> if yes we print the address of the register
                printf("Rn:%p  ", &Register[Operand_1]);
            }
            if (Second[0] == 'R' || Second[1] == 'R')
            { // looking if Second(Rm) a register --> if yes we print the address of the register
                printf("Rm:%p", &Register[Operand_2]);
            }
            printf("\n");
        }
        else
        {
            zeroit(Register, NumberReg);                                                        // puts all values of the Register and Data array to zero
            zeroit(Data, NumberData);
        }
        printRegDat(Register, Data);
        reading = lineRead(Program, Operator, First, Second);                                      // reading the file
        if (count != 0 && count < 16)
        {
            DataFilePrint(Data, fpData);
        }
        count++;
        convertToBinary = assamblerToOp(Operator, First, Second, &Opcode, &Operand_1, &Operand_2); // converting the input of the file to binary
        if (convertToBinary != 1)
        {
            printf("\n\nInvalid Operation!");
            break;
        }

    } while ((reading) & (PC < 255));

    Output(Register, HexFile);
    //Output(Data, TextFile);

    free(Register); // Freeing Register
    free(Data);     // Freeing Data
    fclose(Program);
    fclose(fpData);
    printf("\n\nEnd");

    return 0;
}
