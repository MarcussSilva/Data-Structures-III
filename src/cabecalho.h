#ifndef CABECALHO_H
#define CABECALHO_H

#include "registro.h"

typedef struct registroCabecalho{
    char status;
    int proxRRN;
    int nroTecnologias;
    int nroParesTecnologias;
}registroCab;

int createCabecalho(registroCab *rC);
void readCabecalho(registroCab *rC, FILE *dataBinFile);
void setCabecalho(registroCab *rC, int numParesUnicos, int numTecnologiasUnicas);
void writeCabecalho(FILE *binFile, registroCab *rC);
void verifyStatus(registroCab rC);

void addTecnologiaUnica(char tecUnic[][MAX_STRING], char *tecnologia, int tamanho, int *numTec);
void addParUnico(char parUnic[][2][MAX_STRING], registro r1, int *numPares);
#endif