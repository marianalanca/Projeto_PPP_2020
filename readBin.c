/*
 * Projeto realizado por Mariana Lança 2018288500
 *
 * Este ficheiro tem como objetivo imprimir na consola a informação dos ficheiros binários gerados pelo main.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH "dataFiles/"

#define MAXLINE 100
#define MAXNAME 50

//region Declaração das variáveis do tipo struct
struct partnerNode{
    char name[MAXNAME];
    struct partnerNode *next;
};

struct sharesNode{
    double shareValue;
    struct sharesNode *next;
};

// Estrutura com os dados todos do cliente
struct dataClient{
    char name[MAXNAME];
    double shareValue;
};

//endregion

void printDue(FILE*);
void printPaid(FILE*);

int main() {
    //Declaração e inicialização das variáveis necessárias à abertura dos ficheiros
    FILE *dueFile = NULL;
    FILE *paidFile = NULL;

    char* fileName = (char*)malloc(MAXLINE);
    char *filePath = (char*)malloc(MAXLINE);

    //É pedido que seja inserido o nome do ficheiro já existente, que foi criado anteriormente; Enquanto não for possível
    //Enquanto não se conseguir abrir o ficheiro, continua-se a pedir os dados
    while(paidFile == NULL) {
        printf("Open paidFile: ");
        fgets(fileName,MAXLINE,stdin);
        if (*fileName=='\n'){
            fprintf(stderr,"Invalid name\n");
            continue;
        }
        fileName[strlen(fileName)-1] = '\0';
        sprintf(filePath,"%s%s.bin",PATH,fileName);
        paidFile = fopen(filePath, "rb");
        if (paidFile == NULL)
            fprintf(stderr,"File does not exists\n");
    }

    //Limpeza da informação guardada na variável fileName, de modo a guardar nova informação
    memset(fileName,0,strlen(fileName));

    //É pedido que seja inserido o nome do ficheiro já existente, que foi criado anteriormente; Enquanto não for possível
    // abrir-se o ficheiro, continua-se a pedir os dados
    while(dueFile == NULL) {
        printf("Open dueFile: ");
        fgets(fileName,MAXLINE,stdin);
        if (*fileName=='\n'){
            fprintf(stderr,"Invalid name\n");
            continue;
        }
        fileName[strlen(fileName)-1] = '\0';
        sprintf(filePath,"%s%s.bin",PATH,fileName);
        dueFile = fopen(filePath, "rb");
        if (dueFile == NULL)
            fprintf(stderr,"File does not exists\n");
    }


    //imprimir estruturas
    printf("#------------DATA PAID------------#\n");
    printPaid(paidFile);
    printf("#------------DATA DUE--------------#\n");
    printDue(dueFile);

    //Libertação de toda a memória alocada
    free(fileName);
    fileName = NULL;

    free(filePath);
    filePath = NULL;

    fclose(paidFile);
    fclose(dueFile);

    return 0;
}

/**--------------------------------------------------------------------------------**/

/*
 * ENTRADA: Função recebe o ficheiro do qual se pretende ler os dados das estruturas
 * SAIDA: Função não devolve valor algum
 */
void fileDue(FILE* binFile){
    //Declaração da variável do tipo struct dataDue
    struct dataClient node;

    //Imprimem-se os parâmetros da estrutura, enquanto for possível ler do ficheiro
    while (fread(&node,sizeof(node),1,binFile)==1){
        printf("%-20s\t%.2lf\n",node.name,node.shareValue);
    }
}

/*
 * ENTRADA: Função recebe o ficheiro do qual se pretende ler os dados das estruturas
 * SAIDA: Função não devolve valor algum
 */
void printDue(FILE* file){
    fileDue(file);
}

/*
 * ENTRADA: Função recebe o ficheiro do qual se pretende ler os dados das estruturas
 * SAIDA: Função não devolve valor algum
 */
void filePaid(FILE* binFile){
    //Declaração da variável do tipo struct dataPaid
    struct dataClient node;

    //Imprimem-se os parâmetros da estrutura, enquanto for possível ler do ficheiro
    while (fread(&node,sizeof(node),1,binFile)==1){
        printf("%-20s\t%.2lf\n",node.name,node.shareValue);
    }
}

/*
 * ENTRADA: Função recebe o ficheiro do qual se pretende ler os dados das estruturas
 * SAIDA: Função não devolve valor algum
 */
void printPaid(FILE* file){
    filePaid(file);
}