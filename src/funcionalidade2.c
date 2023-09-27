#include "registro.h"

void funcionalidade2(char *dataBin) {
    FILE *dataBinFile = fopen(dataBin, "rb");

    if (!dataBinFile) {
        printf("Erro ao abrir arquivo\n");
        return;
    }

    registro *r;
    registroCab *rc;

    r = malloc(sizeof(registro));

    rc = malloc(sizeof(registroCab));

    fread(&rc->status, sizeof(char), 1, dataBinFile);
    fread(&rc->proxRRN, sizeof(int), 1, dataBinFile);
    fread(&rc->nroTecnologias, sizeof(int), 1, dataBinFile);
    fread(&rc->nroParesTecnologias, sizeof(int), 1, dataBinFile);

    while (!feof(dataBinFile)) {

        fread(&r->removido, 1, 1, dataBinFile);
        if (feof(dataBinFile)) {
            free(r);
            break;
        }

        fread(&r->grupo, sizeof(int), 1, dataBinFile);
        if(r->grupo == -1){
            char nome[4] = "NULO";
            //r->grupo = strdup(nome);
        }

        fread(&r->popularidade, sizeof(int), 1, dataBinFile);
        if(r->popularidade == -1){
            char nome[4] = "NULO";
            //r->popularidade = strdup(nome);
        }

        fread(&r->peso, sizeof(int), 1, dataBinFile);
        if(r->peso == -1){
            char nome[4] = "NULO";
            //r->peso = strdup(nome);
        }

        fread(&r->tamTecnologiaOrigem, sizeof(int), 1, dataBinFile);
        r->nmTecnologiaOrigem = malloc(r->tamTecnologiaOrigem);
        fread(r->nmTecnologiaOrigem, 1, r->tamTecnologiaOrigem, dataBinFile);

        if(r->tamTecnologiaOrigem == 1){
            char nome[4] = "NULO";
            r->nmTecnologiaOrigem = nome; 
        }

        fread(&r->tamTecnologiaDestino, sizeof(int), 1, dataBinFile);
        r->nmTecnologiaDestino = malloc(r->tamTecnologiaDestino);
        fread(r->nmTecnologiaDestino, 1, r->tamTecnologiaDestino, dataBinFile);

        if(r->tamTecnologiaDestino == 0){
            char nome[4] = "NULO";
            r->nmTecnologiaDestino = nome; 
        }

        printf("%s, %d, %d, %s, %d \n", r->nmTecnologiaOrigem, r->grupo, r->popularidade, r->nmTecnologiaDestino, r->peso);

        int deslocamento = TAMREGISTRO - TAMREGISTROFIXO - r->tamTecnologiaOrigem - r->tamTecnologiaDestino;
        fseek(dataBinFile, deslocamento, SEEK_CUR);
    }

    free(r->nmTecnologiaOrigem);
    free(r->nmTecnologiaDestino);
    free(rc);
    fclose(dataBinFile);
}
