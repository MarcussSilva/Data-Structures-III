#include "funcionalidades.h"
#include "complemento.h"
#include "cabecalho.h"
#include "registro.h"

/*
*               TRABALHO INTRODUTORIO 
*           Estrutura de Dados III (SCC0607)
*
*       Rodrigo Rodrigues de Castro - 13695362
*         Marcus Vinicius da Silva - 13833150
*/ 

int main(){
    int command;
    scanf("%d", &command);

    switch (command){
        case 1:
            funcionalidade1();
            break;
        case 2:
            funcionalidade2();
            break;
        case 3:
            funcionalidade3();
            break;
        case 4:
            funcionalidade4();
            break;
        default:
            printf("Falha no processamento do arquivo.\n");
            break;
    }
    return 0;
}







