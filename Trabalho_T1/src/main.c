#include "funcionalidades.h"

/*
*                    TRABALHO 1 
*           Estrutura de Dados III (SCC0607)
*
*       Rodrigo Rodrigues de Castro - 13695362
*         Marcus Vinicius da Silva - 13833150
*/ 

int main(){
    int command;
    scanf("%d", &command);

    switch (command){
        case 5:
            funcionalidade5();
            break;
        case 6:
            funcionalidade6();
            break;
        case 7:
            funcionalidade7();
            break;
        default:
            printf("Falha no processamento do arquivo.\n");
            break;
    }
    return 0;
}