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

#include "functions.h"

static struct partnerNode* clientsList;
static struct sharesNode* sharesList;
static struct paidList* paidList;
static struct dueList* dueList;

//region LISTS

//region Input Lists
/*
 * ENTRADA: Função não tem parâmetros de entrada
 * SAIDA: Função não devolve nenhum valor
 */
void initializeLists(void){
    //Inicializar as listas ligadas de entrada, struct dataPartner e struct dataShare, com o valor NULL
    clientsList = NULL;
    sharesList = NULL;
}

/*
 * ENTRADA: Função recebe como parâmetro de entrada o nome do sócio a adicionar
 * SAIDA: Devolve true no caso de elemento ser criado e adicionado à lista ligada; false se ocorrer algum erro que impeça
 * entrada do elemento na lista.
 */
bool addPartner(char* name){
    struct partnerNode *aux; //declaração de variável auxiliar do tipo struct dataPartner*

    aux = (struct partnerNode *) malloc(sizeof(struct partnerNode)); //alocação da memória necessária para esse nó

    if (aux == NULL){ //Verificar se alocação correu bem; se não, enviar mensagem de erro para o stderr e a função termina
        fprintf(stderr,"There is no space for element");
        return false;
    }

    strcpy(aux->name,name); //Copiar nome para a variável auxiliar
    // Guardar a variável como head da lista ligada
    aux->next = clientsList;
    clientsList = aux;

    return true;
}

/*
 * ENTRADA: Função recebe como parâmetro de entrada o ponteiro para nó do tipo struct partnerNode a ser impresso
 * SAIDA: Função não devolve nenhum valor
 */
void printPartners(struct partnerNode *node){
    if (node!=NULL){
        printf("%s\n",node->name); //Impressão do valor do nó
        printPartners(node->next); //A função é chamada recursivamente, com o valor do nó seguinte
    }
}

/*
 * ENTRADA: Função recebe como parâmetro de entrada o ponteiro para nó do tipo struct partnerNode a ser eliminado
 * SAIDA: Função não devolve nenhum valor
 */
void deletePartners(struct partnerNode *node){
    if (node!=NULL){
        deletePartners(node->next); //Função é chamada recursivamente, com o valor do nó seguinte
        //É limpo o valor do nó
        free(node);
        node = NULL;
    }
}

/*
 * ENTRADA: Função recebe como parâmetro de entrada o valor já pago pelo sócio a adicionar
 * SAIDA: Devolve true no caso de elemento ser criado e adicionado à lista ligada; false se ocorrer algum erro que impeça
 * entrada do elemento na lista.
 */
bool addShare(double shareValue){
    struct sharesNode *aux; //declaração de variável auxiliar do tipo struct dataShare*

    aux = (struct sharesNode *) malloc(sizeof(struct sharesNode)); //alocação da memória necessária para esse nó

    if (aux == NULL){ //Verificar se alocação correu bem; se não, enviar mensagem de erro para o stderr e a função termina
        fprintf(stderr,"There is no space for element");
        return false;
    }

    //construição do novo nó da fila
    aux->shareValue = shareValue;
    // Guardar a variável como head da lista ligada
    aux->next = sharesList;
    sharesList = aux;

    return true;
}

/*
 * ENTRADA: Função recebe como parâmetro de entrada o ponteiro para nó do tipo struct sharesNode a ser impresso
 * SAIDA: Função não devolve nenhum valor
 */
void printShares(struct sharesNode* node){
    if (node!=NULL){
        printf("%.2lf\n",node->shareValue); //Impressão do valor do nó
        printShares(node->next); //A função é chamada recursivamente, com o valor do nó seguinte
    }
}

/*
 * ENTRADA: Função recebe como parâmetro de entrada o ponteiro para nó do tipo struct sharesNode a ser eliminado
 * SAIDA: Função não devolve nenhum valor
 */
void deleteShares(struct sharesNode *node){
    if (node!=NULL){
        deleteShares(node->next); //Função é chamada recursivamente, com o valor do nó seguinte
        //É limpo o valor do nó
        free(node);
        node = NULL;
    }
}

/*
 * ENTRADA: Função não tem parâmetros de entrada
 * SAIDA: Função não devolve nenhum valor
 */
void initializeResList(void) {
    //Inicializar as listas ligadas de entrada, struct paidList e struct dueList, com o valor NULL
    paidList = NULL;
    dueList = NULL;
}

/*
 * ENTRADA: Função recebe como parâmetro de entrada o nó que contém o nome do sócio sem cotas em atraso e o nó com o
 * valor resultante após serem tirados os 50 euros
 * SAIDA: Devolve true no caso de elemento ser criado e adicionado à lista ligada; false se ocorrer algum erro que impeça
 * entrada do elemento na lista.
 */
bool addPaid(char* partner, double value){
    struct paidList * aux, *next, *prev; //declaração de variável auxiliar do tipo struct paidList*

    //Obter espaço para um novo nó
    aux = (struct paidList *) malloc(sizeof(struct paidList));
    if (aux == NULL){ //Verificar se alocação correu bem; se não, enviar mensagem de erro para o stderr e a função termina
        fprintf(stderr,"There is no space for element");
        return false;
    }

    //construição do novo nó da fila
    strcpy(aux->client.name, partner);
    aux->client.shareValue = value;
    aux->next = NULL;

    //Procurar a posição onde a mensagem deve ficar
    if (paidList == NULL) { //Se a lista ligada estiver vazia, guarda-se no início da mesma
        paidList = aux;
    } else {
        // fila contém nós; Pretende-se organizar a fila por ordem alfabética, logo é necessário verificar os valores de cada nó
        if (strcmp(paidList->client.name,partner)>=0) {
            // inserir à entrada da lista
            aux->next = paidList;
            paidList = aux;
        } else {
            // procurar posição de inserção
            prev = paidList;
            next = paidList->next;
            while (next != NULL && strcmp(next->client.name,partner)<0) { //Percorre-se lista até se encontrar a posição
                prev = next;
                next =next->next;
            }
            if (next == NULL) {
                // inserir à saída da lista
                prev->next = aux;
            } else {
                // inserir a meio da lista
                prev->next = aux;
                aux->next = next;
            }
        }
    }
    return true;
}

/*
 * ENTRADA: Função recebe como parâmetro de entrada o ponteiro para nó do tipo struct paidList a ser impresso
 * SAIDA: Função não devolve nenhum valor
 * TRANSFORMAÇÃO: Imprime recursivamente os parâmetros do nó, até encontrar um com valor NULL
 */
void printPaid(struct paidList *node){
    if (node!=NULL){
        printf("%s\t%.2lf\n",node->client.name,node->client.shareValue); //Impressão do valor do nó
        printPaid(node->next); //A função é chamada recursivamente, com o valor do nó seguinte
    }
}

/*
 * ENTRADA: Função recebe como parâmetro de entrada o ponteiro para nó do tipo struct paidList a ser eliminado
 * SAIDA: Função não devolve nenhum valor
 * TRANSFORMAÇÃO: Função limpa a lista recursivamente
 */
void deletePaid(struct paidList *node){
    if (node!=NULL){
        deletePaid(node->next); //Função é chamada recursivamente, com o valor do nó seguinte
        //É limpo o valor do nó
        free(node);
        node = NULL;
    }
}

/*
 * ENTRADA: Função recebe como parâmetro de entrada o nó que contém o nome do sócio com cotas em atraso e o nó com o
 * valor resultante após serem tirados os 50 euros
 * SAIDA: Devolve true no caso de elemento ser criado e adicionado à lista ligada; false se ocorrer algum erro que impeça
 * entrada do elemento na lista.
 */
bool addDue(char* partner, double value){
    struct dueList * aux, *next, *prev; //declaração de variáveis auxiliares do tipo struct dueList*

    //alocação da memória necessária para esse nó
    aux = (struct dueList *) malloc(sizeof(struct dueList));
    if (aux == NULL){ //Verificar se alocação correu bem; se não, enviar mensagem de erro para o stderr e a função termina
        fprintf(stderr,"There is no space for element");
        return false;
    }

    //construição do novo nó da fila
    strcpy(aux->client.name,partner);
    aux->client.shareValue = value;
    aux->next = NULL;

    //Procurar a posição onde a mensagem deve ficar
    if (dueList == NULL) {
        dueList = aux; //Se a lista ligada estiver vazia, guarda-se no início da mesma
    } else {
        // fila contém nós; Pretende-se organizar a fila por ordem alfabética, logo é necessário verificar os valores de cada nó
        if (strcmp(dueList->client.name,partner)>=0) {
            // inserir à entrada da lista
            aux->next = dueList;
            dueList = aux;
        } else {
            // procurar posição de inserção
            prev = dueList;
            next = dueList->next;
            while (next != NULL && strcmp(next->client.name,partner)<0) { //Percorre-se lista até se encontrar a posição
                prev = next;
                next = next->next;
            }
            if (next == NULL) {
                // inserir à saída da lista
                prev->next = aux;
            } else {
                // inserir a meio da lista
                prev->next = aux;
                aux->next = next;
            }
        }
    }
    return true;
}

/*
 * ENTRADA: Função recebe como parâmetro de entrada o ponteiro para nó do tipo struct dueList a ser impresso
 * SAIDA: Função não devolve nenhum valor
 */
void printDue(struct dueList *node){
    if (node!=NULL){
        printf("%s\t%.2lf\n",node->client.name,node->client.shareValue); //Impressão do valor do nó
        printDue(node->next); //A função é chamada recursivamente, com o valor do nó seguinte
    }
}

/*
 * ENTRADA: Função recebe como parâmetro de entrada o ponteiro para nó do tipo struct dueList a ser eliminado
 * SAIDA: Função não devolve nenhum valor
 */
void deleteDue(struct dueList *node){
    if (node!=NULL){
        deleteDue(node->next); //Função é chamada recursivamente, com o valor do nó seguinte
        //É limpo o valor do nó
        free(node);
        node = NULL;
    }
}

/*
 * ENTRADA: Função não recebe nenhum valor
 * SAIDA: Função não recebe nenhum valor
 */
void destroyData(void){

    //São chamadas as funções para cada eliminar cada uma das funções, de modo a limpar a memória no final do programa
    deletePartners(clientsList);
    deleteShares(sharesList);
    deletePaid(paidList);
    deleteDue(dueList);

}


void addToLists(void){

    struct partnerNode* clientNode =  clientsList;
    struct sharesNode * shareNode = sharesList;

    //Percorrer as duas listas ligadas na sua totalidade (estas terão o mesmo tamanho, logo é necessário verificar apenas
    //quando se chega ao final de uma delas)
    while (clientNode!=NULL){

        shareNode->shareValue -= FEE; //É retirado o valor da quota

        //Faz-s a comparação, se o valor da quota for menor do que zero, o sócio é adicionado à lista de sócios com quotas
        // em atraso, se não, é adicionado à lista de sócios com quotas já pagas
        if(shareNode->shareValue<0){
            shareNode->shareValue *= (-1); //É feito o módulo do valor
            addDue(clientNode->name,shareNode->shareValue);
        } else{
            addPaid(clientNode->name,shareNode->shareValue);
        }

        //Os ponteiros dos nós são movidos para o nó seguinte, de modo a fazer-se o tratamento desses dados
        clientNode = clientNode->next;
        shareNode = shareNode->next;
    }
    return;
}

/*
 * ENTRADA: Recebe ponteiro do nó do tipo struct paidList a guardar no ficheiro binário e o ficheiro
 * SAIDA: Função não retorna qualquer valor
 */
void paidFile (struct paidList * node, FILE* file){
    //Função é feita de forma recursiva, logo é necessário verificar se o nó é diferente de NULL
    if (node!=NULL){
		assert(fwrite(&node->client,sizeof(struct dataClient),1,file)==1); //Guardar a estrutura do nó no ficheiro; 
		//Se ocorrer um erro dá-se um assert 
		paidFile(node->next,file); //Se não ocorrer nenhum erro, chama-se a função, passando, como parâmetro, o nó 
        //seguinte, de modo a guardá-lo também, e assim sucessivamente
    }
    return;
}

void writePaid (FILE* file){
    paidFile(paidList, file);
    return;
}

/*
 * ENTRADA: Recebe ponteiro do nó do tipo struct dueList a guardar no ficheiro binário e o ficheiro
 * SAIDA: Função não retorna qualquer valor
 */
void dueFile (struct dueList * node, FILE* file){
    //Função é feita de forma recursiva, logo é necessário verificar se o nó é diferente de NULL
    if (node!=NULL){
        assert(fwrite(&node->client,sizeof(struct dataClient),1,file)==1);//Guardar a estrutura do nó no ficheiro;
		//Se ocorrer um erro dá-se um assert 
        dueFile(node->next,file);//Se não ocorrer nenhum erro, chama-se a função, passando, como parâmetro, o nó 
        //seguinte, de modo a guardá-lo também, e assim sucessivamente
    }
    return;
}

void writeDue (FILE* file){
    dueFile(dueList, file);
    return;
}

