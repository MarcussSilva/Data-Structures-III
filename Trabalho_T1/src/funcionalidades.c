#include "indice.h"
#include "registro.h"
#include "cabecalho.h"
#include "arvore.h"
#include "funcionalidades.h"
#include "complemento.h"

void funcionalidade5(){
    
    // Inicializa e abre os arquivos binários com base na entrada
    char *dataBIN = malloc(sizeof(char) * 40);
    char *dataINDEX = malloc(sizeof(char) * 40);
    scanf("%s %s", dataBIN, dataINDEX);
    
    FILE *dataFile = fopen(dataBIN, "rb");
    checkFile(dataFile);

    FILE *indexFile = fopen(dataINDEX, "wb+");
    checkFile(indexFile);

    // Criação do Cabeçalho do arquivo de Indices
    cabIndice *indexCab = createCabecalhoIndice();
    writeCabecalhoIndice(indexFile, indexCab);
    
    // Verificação do Cabeçalho do arquivo de Dados
    registroCab rC;
    readCabecalho(&rC, dataFile);
    verifyStatus(rC);

    // Criacao e definição do Primeiro No
    No *no = criarNo();
    int rrnDados = 0;
    registro *r0 = malloc(sizeof(registro));
    createRegistro(r0);
    readRegistro(r0, dataFile);
    no->RRNdoNo = 0;
    no->vetChaves[0].chave = createChave(r0);
    no->vetChaves[0].referencia = rrnDados;
    no->nroChavesNo = 1;
    rrnDados++;

    // Inserindo o primeiro No no arquivo de Indice, com alterações no cabecalho
    writePagina(indexFile, no, 0);
    indexCab->noRaiz = 0;
    indexCab->RRNproxNo = 1;
    writeCabecalhoIndice(indexFile, indexCab);

    // Variaveis utilizadas na função de Inserção
    Chave *promoChave = malloc(sizeof(Chave));
    int *promoRFilho = malloc(sizeof(int));

    // Loop de Inserção
    int flag = 0;
    while(flag != -1){    
        registro *r = malloc(sizeof(registro));
        int aux = readRegistro(r, dataFile);
        
        // Fim do arquivo
        if(aux == 0){
            flag = -1;
            break;
        }

        // Registro Removido
        if(aux == -1){
            rrnDados++;
            continue;
        }

        // Registro com campo Nulo
        if(r->tamTecnologiaDestino == 0 || r->tamTecnologiaOrigem == 0){
            rrnDados++;
            continue;
        }

        Chave chaveI;
        chaveI.chave = createChave(r);
        chaveI.referencia = rrnDados;
        rrnDados++;

        // Realiza inserção
        int rrnRaiz = indexCab->noRaiz;
        int promo = inserirArvore(indexFile, &rrnRaiz, &chaveI, promoRFilho, promoChave);
       
        // Realiza promoção se necessario
        if (promo == PROMOTION) {
            realizaPromocao(indexFile, indexCab, promoRFilho, promoChave);
        }

        flag++;
    }
    
    // Atualizao status e fecha os arquivos
    readCabIndice(indexFile, indexCab);
    indexCab->status = '1';
    writeCabecalhoIndice(indexFile, indexCab);

    fclose(indexFile);
    fclose(dataFile); 
    
    // Usando a função fornecida e liberando memória
    binarioNaTela(dataINDEX);

    free(no);
    free(r0);
    free(dataBIN);
    free(promoChave);
    free(promoRFilho);
    free(indexCab);
}


void funcionalidade6(){
    // Inicializa o arquivo de dados e o de índices e verifica se eles existem

    char *dataBIN = malloc(sizeof(char) * 40);
    char *dataINDEX = malloc(sizeof(char) * 40);
    int n = 0;
    scanf("%s %s %d", dataBIN, dataINDEX, &n);

    FILE *dataFile = fopen(dataBIN, "rb");
    checkFile(dataFile);

    FILE *indexFile = fopen(dataINDEX, "rb");
    checkFile(indexFile);

    // Cria o cabeçalho do arquivo de índices
    cabIndice *indexCab = createCabecalhoIndice();
    readCabIndice(indexFile, indexCab);
    verifyStatusIndice(*indexCab);

    // Strings temporárias
    char tmp1[MAX_STRING];
    char tmpT[MAX_STRING];

    int rrnDados = indexCab->noRaiz;

    // Cria o cabeçalho do arquivo de dados
    registroCab *rC = malloc(sizeof(registroCab));
    createCabecalho(rC);
    readCabecalho(rC, dataFile);
    verifyStatus(*rC);

    int buscaDados = -1;

    // For para realizar a quantidade de buscas solicitadas
    for(int i = 0; i < n; i++){
        Chave chaveBusca, chaveBuscaF3;
        
        // Strings para armazenar as entradas
        chaveBusca.chave = malloc(sizeof(char)*MAX_STRING);
        chaveBuscaF3.chave = malloc(sizeof(char)*MAX_STRING);

        scanf(" %s", tmp1);

        // Retira as aspas das strings caso alguma entrada tenha aspas
        if(strcmp(tmp1,"nomeTecnologiaOrigemDestino") == 0 || strcmp(tmp1,"nomeTecnologiaOrigem") == 0 || strcmp(tmp1,"nomeTecnologiaDestino") == 0){
            scan_quote_string(tmpT);
        }else{
            scanf("%s ", tmpT);
        }

        // Atribui o valor das strings temporárias para as chaves de busca
        // Também são colocados '$' em uma das chaves para facilitar a busca na árvore B
        strcpy(chaveBuscaF3.chave, tmpT);
        
        int len = strlen(tmpT);
        for(int i = len; i < MAX_STRING; i++){
            tmpT[i] = '$';
        }
        strcpy(chaveBusca.chave,tmpT);

        // If e else para verificar se a busca será feita na árvore ou no arquivo de dados
        if(strcmp(tmp1, "nomeTecnologiaOrigemDestino") == 0){
            buscaArvore(indexFile, dataFile, &rrnDados, &buscaDados, &chaveBusca);
        }else{
            funcionalidade3(dataFile,tmp1, chaveBuscaF3.chave);
        }
    }

    // Atualizando o status do arquivo e fechando eles
    indexCab->status = '1';
    writeCabecalhoIndice(indexFile, indexCab);

    fclose(indexFile);
    fclose(dataFile);

    // Liberando memória
    free(rC);
    free(indexCab);
    free(dataBIN);
    free(dataINDEX);
}

void funcionalidade7(){

    // Inicializa o arquivo de dados e o de índices e verifica se eles existem
    char *dataBIN = malloc(sizeof(char) * 40);
    char *dataINDEX = malloc(sizeof(char) * 40);
    int n = 0;
    scanf("%s %s %d", dataBIN, dataINDEX, &n);

    FILE *dataFile = fopen(dataBIN, "rb+");
    checkFile(dataFile);

    FILE *indexFile = fopen(dataINDEX, "rb+");
    checkFile(indexFile);

    // Cria o cabeçalho do arquivo de índices
    cabIndice *indexCab = createCabecalhoIndice();
    readCabIndice(indexFile, indexCab);
    verifyStatusIndice(*indexCab);
    indexCab->status='0';
    writeCabecalhoIndice(indexFile, indexCab);
    fseek(indexFile, 0, SEEK_SET);

    //C ria o cabeçalho do arquivo de dados
    registroCab *rC = malloc(sizeof(registroCab));
    createCabecalho(rC);
    readCabecalho(rC, dataFile);
    verifyStatus(*rC);
    rC->status='0';
    fseek(dataFile, 0, SEEK_SET);
    fwrite(&rC->status, sizeof(char), 1, dataFile);

    // Encontrando o ultimo RRN do arquivo de dados
    fseek(dataFile, 0, SEEK_END);
    int tamanho = ftell(dataFile);// num bytes
    tamanho = (tamanho - 12)/TAMREGISTRO;// rrn final

    // Loop para escrever no arquivo de dados e armazenar as chaves
    Chave *vetChave = malloc(sizeof(Chave)*n);
    for(int j = 0; j<n; j++){
        // Paginas no arquivo de  dados
        registro *registroInsercao = malloc(sizeof(registro));
        createRegistro(registroInsercao);

        // Leitura de entrada
        scanfEntrada(registroInsercao);

        verificarTecnologias(dataFile, *registroInsercao);
        fseek(dataFile, 0, SEEK_END);
        writeRegistro(registroInsercao, dataFile);

        // Armazenamento das chaves
        if(registroInsercao->tamTecnologiaDestino == 0 || registroInsercao->tamTecnologiaOrigem == 0 ){
            vetChave[j].chave = "";
            vetChave[j].referencia = -1;
        }else{
            vetChave[j].chave = createChave(registroInsercao);
            vetChave[j].referencia = tamanho;
        }
        tamanho++;
    }

    //Loop para inserir no arquivo de indices
    Chave *promoChave = malloc(sizeof(Chave));
    int *promoRFilho = malloc(sizeof(int));
    for(int i = 0; i<n; i++){
        // Pagina no arquivo de Indice
        if(vetChave[i].referencia != -1){
            readCabIndice(indexFile, indexCab);

            // Realiza inserção
            int rrnRaiz = indexCab->noRaiz;
            int promo = inserirArvore(indexFile, &rrnRaiz, &vetChave[i], promoRFilho, promoChave);
        
            // Realiza promocao se necessario
            if (promo == PROMOTION) {
                realizaPromocao(indexFile, indexCab, promoRFilho, promoChave);
            }
        }
    }

    // Atualizando os status dos arquivos e fechando-os
    indexCab->status = '1';
    fseek(indexFile, 0, SEEK_SET);
    fwrite(&indexCab->status, sizeof(char), 1, indexFile);

    rC->status = '1';
    fseek(dataFile, 0, SEEK_SET);
    fwrite(&rC->status, sizeof(char), 1, dataFile);

    fclose(indexFile);
    fclose(dataFile);

    // Usando as funções fornecidas e liberando memória
    binarioNaTela(dataBIN);
    binarioNaTela(dataINDEX);

    free(rC);
    free(indexCab);
    free(dataBIN);
    free(dataINDEX);
}


// ---------------------- TRABALHO 0 ---------------------------------
void funcionalidade1(){

    // Inicializa e abre os arquivos csv e binário
    char *dataBIN = malloc(sizeof(char)*40);
    char *dataCSV = malloc(sizeof(char)*40);
    scanf("%s %s", dataCSV, dataBIN);

    FILE *csvFile = fopen(dataCSV, "r");
    checkFile(csvFile);

    FILE *binFile = fopen(dataBIN, "wb");
    checkFile(binFile);

    // Cria o cabeçalho do arquivo
    registroCab rC;
    createCabecalho(&rC);
    writeCabecalho(binFile, &rC);
    
    // Declaração da variaveis responsaveis pelo armazenamento em memoria primaria
    char tecUnic[MAX_TECNOLOGIAS][MAX_STRING];
    int numTecnologias = 0;
    char parUnic[MAX_TECNOLOGIAS][2][MAX_STRING];
    int numPares = 0;

    // Pula a primeira linha do arquivo
    char linha[100];
    fgets(linha,sizeof(linha) , csvFile);

    // Loop para ler o arquivo .csv e escrever no arquivo .bin
    while (fgets(linha, sizeof(linha), csvFile)) {
        int posicao = 0;
        registro *r1 = malloc(sizeof(registro));
        createRegistro(r1);

        // Definição dos registro e escrita no arquivo binario
        setRegistro(r1, linha, &posicao);
        writeRegistro(r1, binFile);
        
        // Funções que armazenam dados de tecnologia recém adicionados 
        addTecnologiaUnica(tecUnic, r1->nmTecnologiaOrigem, r1->tamTecnologiaOrigem, &numTecnologias);
        addTecnologiaUnica(tecUnic, r1->nmTecnologiaDestino, r1->tamTecnologiaDestino, &numTecnologias);
        addParUnico(parUnic, *r1, &numPares);

        rC.proxRRN = rC.proxRRN + 1;

        freeRegistro(r1);
    }

    // Atualizando o cabeçalho com novos valores
    setCabecalho(&rC, numPares, numTecnologias);
    writeCabecalho(binFile, &rC);

    // Fecha os arquivos
    fclose(csvFile);
    fclose(binFile);

    binarioNaTela(dataBIN);

    // Libera memoria referentes aos nomes dos arquivos
    free(dataBIN);
    free(dataCSV);
}

void funcionalidade2() {
    // Inicializa o arquivo binário  e verifica se ele existe
    char *dataBin = malloc(sizeof(char)*40);
    scanf("%s", dataBin);

    FILE *binFile = fopen(dataBin, "rb");
    checkFile(binFile);
    

    // Lê o cabeçalho do arquivo e verifica o status
    registroCab rC;
    readCabecalho(&rC, binFile);
    verifyStatus(rC);

    int flag = 0;
    // Percorre o arquivo e imprime-o
    while(flag != -1){
        // Criação do registro
        registro *r = malloc(sizeof(registro));
        createRegistro(r);

        // Função que lê um registro do arquivo e para quando chagar ao fim
        if(readRegistro(r, binFile) == 0){
            if(flag == 0){
                printf("Registro inexistente.\n");
            }
            flag = -1;
            freeRegistro(r);
            break;
        }

        // Função que imprime um registro do arquivo
        printRegistro(*r);
        freeRegistro(r);
        flag = 1;   
    }

    // Fecha o arquivo
    closeFile(binFile, dataBin);
}

void funcionalidade3(FILE *binFile,char *tmp1, char *tmp2){

    // Dados de entrada
    int contador = 0;
    listBuscas dadosBuscas[MAX_TECNOLOGIAS];
    //int n = 1;
    strcpy(dadosBuscas[contador].nomeCampo,tmp1);
    strcpy(dadosBuscas[contador].valorCampo,tmp2);

    int flag;
    // For para fazer as buscas solicitadas

    int i = 0;
    flag = 0;
    // For que percorre o arquivo para encontrar os registros
    for(int j = 0; j<MAX_TECNOLOGIAS; j++){
        int registroEncontrado = 0;

        // Criação dos registros
        registro *r1 = malloc(sizeof(registro)+1);
        createRegistro(r1);

        // Lê um registro do arquivo binário e encerra o loop no fim do arquivo
        if(readRegistro(r1, binFile) == 0){
            freeRegistro(r1);
            break;
        }

        // Cadeia de if e else para verificar a existência dos campos do registro
        if (strcmp(dadosBuscas[i].nomeCampo, "nomeTecnologiaOrigem") == 0 && strcmp(r1->nmTecnologiaOrigem, dadosBuscas[i].valorCampo) == 0) {
            registroEncontrado = 1;
        } else if (strcmp(dadosBuscas[i].nomeCampo, "nomeTecnologiaDestino") == 0 && strcmp(r1->nmTecnologiaDestino, dadosBuscas[i].valorCampo) == 0) {
            registroEncontrado = 1;
        } else if (strcmp(dadosBuscas[i].nomeCampo, "grupo") == 0 && r1->grupo == atoi(dadosBuscas[i].valorCampo)){
            registroEncontrado = 1;
        } else if (strcmp(dadosBuscas[i].nomeCampo, "popularidade") == 0 && r1->popularidade == atoi(dadosBuscas[i].valorCampo)){
            registroEncontrado = 1;
        } else if (strcmp(dadosBuscas[i].nomeCampo, "peso") == 0 && r1->peso == atoi(dadosBuscas[i].valorCampo)){
            registroEncontrado = 1;
        }

        if(registroEncontrado){
            if(r1->removido == '0'){
                printRegistro(*r1);
                flag = 1;
            }
        }
        freeRegistro(r1);

    }
    if(!(flag))
        printf("Registro inexistente.\n");

    //Retorna ao começo do arquivo
    fseek(binFile, 13, SEEK_SET);
}

void funcionalidade4(FILE *binFile, int *RRN){

    // Define como será feito o cáculo para ir para o próximo RRN
    int rrnDestino = (*RRN*TAMREGISTRO)+13;
    registro *r1 = malloc(sizeof(registro));

    // Busca o registro solicitado no arquivo binário
    if(fseek(binFile, rrnDestino, SEEK_SET) == 0){
        createRegistro(r1);
        if (readRegistro(r1, binFile) == 0){
            printf("Registro inexistente.");
        }else{
            if(r1->removido == '0'){
                printRegistro(*r1);
            }
        }
    }else{
        printf("Registro inexistente.");
    }
 
    // Libera memoria do registro
    freeRegistro(r1);
}