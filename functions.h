/*
 * Licenciatura em Engenharia Informática 2019/2020
 *	  Princípios de Programação Procedimental
 *  
 *            Projeto realizado por 
 * 		     Mariana Lança 2018288500
 *
 * 		
 *
 */

#ifndef CODIGO_FUNCTIONS_H
#define CODIGO_FUNCTIONS_H
#endif //CODIGO_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <unistd.h>

#define MAXLINE 100
#define MAXNAME 50
#define FEE 50 // Valor da quota

#define PATH "dataFiles/" // caminho usado para guardar a localização dos ficheiros

//region Read from File

// Estrutura que contém a informação do nome do cliente

//Lista ligada com os nomes dos clientes
struct partnerNode{
    char name[MAXNAME];
    struct partnerNode *next;
};

//Lista ligada com os valores pagos pelos clientes
struct sharesNode{
    double shareValue;
    struct sharesNode *next;
};

// Estrutura com os dados do cliente
struct dataClient{
    char name[MAXNAME];
    double shareValue;
};
//endregion

//region To save in final file

//Lista ligada com os dados dos clientes com as quotas em dia
struct paidList{
    struct dataClient client;
    struct paidList *next;
};

//Lista ligada com os dados dos clientes com as quotas em atraso
struct dueList{
    struct dataClient client;
    struct dueList *next;
};

//endregion

void initializeLists(void);
void initializeResList(void);

bool addPartner(char* );
bool addShare(double );
bool addPaid(char* , double );
bool addDue(char* , double );

void writePaid (FILE* );
void writeDue (FILE* );
void addToLists(void);

void destroyData(void);


