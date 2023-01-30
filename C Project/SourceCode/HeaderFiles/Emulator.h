//
// Created by Marco on 19.01.2023.
//

#ifndef C_PROJEKT_EMULATORHEADER_H
#define C_PROJEKT_EMULATORHEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define NumberReg 16
#define NumberData 16


uint8_t * Register;
uint8_t * Data;


_Bool lineRead(FILE * Pr, char * Op, char * First, char * Sec);
void printRegDat(uint8_t * Reg, uint8_t * Dat);
_Bool whichMOV(char * First, char * Sec, uint8_t * Opcode, uint8_t * Operand_1, uint8_t * Operand_2);
_Bool assamblerToOp(const char * Op, char * First, char * Sec, uint8_t * Opcode, uint8_t * Operand_1, uint8_t * Operand_2);
uint8_t firstReInBinary(char * number);
uint8_t secReInBinary(char * number);
void DataFilePrint(uint8_t r[16], FILE *fp);
uint8_t orAssembly(uint8_t m, uint8_t n);
uint8_t andAssembly(uint8_t m, uint8_t n);
void funcCall(const uint8_t * Opcode, const uint8_t * Operand_1, const uint8_t * Operand_2, uint8_t * Reg, uint8_t * Dat, uint8_t * PC);
void Output(uint8_t r[16], const char* filename);
void zeroit(uint8_t * Array, uint8_t quantity);
void allocation();

#endif //C_PROJEKT_EMULATORHEADER_H
