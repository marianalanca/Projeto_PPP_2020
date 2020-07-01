/*
 * Licenciatura em Engenharia Informática 2019/2020
 *	  Princípios de Programação Procedimental
 *  
 *            Projeto realizado por 
 * 		     Mariana Lança 2018288500
 *
 *
 * O projeto tem como objetivo analisar a situação do pagamento de quotas dos sócios de um clube de montanhismo.
 *
 * No caso de não se possível a abertura do config.txt, o programa aborta.
 * Se a abertura dos ficheiros a partir do config der erro, o programa fecha, pois ocorreu um erro que pôs em causa a
 * execução do programa.
 * No caso dos ficheiros abertos não terem o mesmo número de linhas, é feito um assert.
 * No caso do formato do nome do sócio ou o seu respetivo valor já pago não tiverem o formato correto, é dado um warning
 * (Invalid characters ou EOF), e os dados desse sócio não são guardados na lista ligada.
 * Se o utilizador não fornecer um nome válido para o ficheiro, é pedido um novo, até ser possível a abertura do ficheiro
 * para escrita.
 * NOTA: Ter em conta que é apenas necessário o nome do ficheiro, sem o caminho nem extensão, visto que esses estão
 * definidos no programa
 */

#include "functions.h"



void readData (FILE* );
void processData(void);
void saveData (void);

int main() {

    //Inicialização do ficheiro config, que contém os caminhos para os restantes ficheiros
    FILE *config = fopen("config.txt","r");

    //Se não se conseguir abrir o config.txt, o programa termina
    assert(config!=NULL);

    //Função que permite abrir os ficheiros, verificar o formato dos seus dados, e guardá-los em listas ligadas
    readData(config);

    //Função irá processar os dados, i.e, retirar o valor das quotas do ano e guardar os dados nas listas ligadas certas
    processData();

    //Após os dados já estarem organizados por sócios com quotas pagas e quotas em atraso, os dados são guardados em
    //ficheiros e as listas destruidas
    saveData();

    //Fecho do config.txt
    fclose(config);

    return 0;
}

//region Main Functions

//region Test lines
/*
 * ENTRADA: Função recebe o ficheiro para ler o número de linhas
 * SAIDA: Função retorna número de linhas do ficheiro
 */
int fileCountLines (FILE* fp){
    int c, count = 0;
    while ((c=getc(fp))!=EOF){
        if (c=='\n')
            count++;
    }

    fseek(fp,0,SEEK_SET); //Voltar ao início do ficheiro de modo a ler os dados
    return count;
}

//region Test Inputs

/*
 * ENTRADA: Função recebe dois char*. O primeiro, input, contém a referência à palavra a ser testada, e o segundo, name,
 * é uma variável com memória pré-alocada, onde será guardado o valor pretendido; Recebe também a linha em que se em que
 * se lê o input.
 * SAIDA: Devolve true no caso do input ter o formato correto; false se o contrário
 */
bool testPartnerInput(char* input, char* name, int line){
    int c;
    int pos = 0;

    if (*input == '\r' || *input == '\n') { //Verificar se input está vazio
        fprintf(stderr, "EOF in line %d\n", line); //No caso de vazio, é enviada uma mensagem para o stderr
        return false; //Função termina
    }

    c = strlen(input);
    if (input[c - 1] != '\n') {
        printf("Overflow in line %d\n", line);
        // limpar o resto da linha
        while ((c = fgetc(stdin)) != EOF && c != '\n');
        return false;
    }

    while ((*input) != '\n') { //Testa-se o char*, caracter a caracter, enquanto o valor for diferente de '/0', que simboliza o fim do char*
        if (isspace(*input) || *input=='\r') //Se se encontrar um espaço em branco, esse é ignorado
            input++;
        else if (isalpha(*input)){ //Se se encontrar uma letra, essa é guardada na variável "name", e avança-se para a próxima posição
            name[pos] = *input;
            pos++;
            input++;
            if (*input=='\r'){
                input++;
            }
            else if(isspace(*input)){ //Se após ser encontrada uma letra houver um espaço, esse é também guardado na variável
                //Isto permite ultrapassar situações em que o input tem vários espaços em branco, pois só se guarda o primeiro, mantendo o formato correto
                name[pos] = *input;
                pos++;
                input++;
            }
        }else{ //Se se encontrar algum caracter que não seja espaço ou letra, é enviada uma mensagem de erro para o stderr e s função termina
            fprintf(stderr,"Invalid characters in line %d\n",line);
            return false;
        }
    }

    name[pos] = '\0';
    return true; //Não ocorreu nenhum erro, o formato é o correto, logo retorna-se true
}

/*
 * ENTRADA: Função recebe um char*, input, contém a referência à palavra a ser testada, um ponteiro para um double, shareValue,
 * onde será guardado o valor da quota a ser testado. Recebe também a linha em que se em que se lê o input.
 * SAIDA: Devolve true no caso do input ter o formato correto; false se o contrário
 */
bool testShareInput(char* input,double *shareValue, int line){
    char *tail;
    int c;

    if (*input == '\r') { //Verificar se input está vazio
        fprintf(stderr,"EOF in line %d\n",line); //No caso de vazio, é enviada uma mensagem para o stderr
        return false; //Função termina
    }

    //Se a linha lida não termina por '\n' a linha era demasiado longa - erro
    c = strlen(input);
    if (input[c - 1] != '\n') {
        printf("Overflow in line %d\n", line);
        // limpar o resto da linha
        while ((c = fgetc(stdin)) != EOF && c != '\n');
        return false;
    }

    errno = 0;
    *shareValue = strtod(input, &tail); //Converter o valor do input num double

    if (errno) {
        fprintf(stderr,"Overflow in long in line %d\n",line); //Se houver situação de erro, houve um overflow
        return false; //a função termina com false
    }

    // verificar que só há espaço branco no resto da linha
    while ((*tail) != 0) {
        if (!isspace(*tail)) {
            fprintf(stderr,"Invalid characters in line %d\n",line); //Se se encontrar algum caracter que não seja número
            return false; //Função termina e devolve false
        }
        tail++;
    }

    return true; //Não ocorreu qualquer erro e é posssível terminar a função a true
}
//endregion

/*
 * ENTRADA:Função recebe o ficheiro config, que contém os dados dos ficheiros a abrir, nomeadamente dois ponteiros, cada
 * um para uma variável do tipo FILE*, filePartner e fileShares, que no final terão o valor dos ficheiros abertos
 * SAIDA: Dunção não dá return de nada
 */
void openFiles(FILE *config, FILE** filePartner, FILE** fileShares){

    char* fileName = (char*) malloc(MAXLINE); //Alocação da memória de fileName,

    fgets(fileName,MAXLINE,config); //Obter do config a primeira linha, de modo a ser possível abrir-se o ficheiro
    assert(fileName!=NULL); //No caso de o ficheiro não conter nenhuma linha, é feito um assert e o programa fecha
    fileName[strlen(fileName)-2] = '\0'; //remove /r/n da string
    *filePartner = fopen(fileName,"r"); //abertura do ficheiro que contém os nomes dos sócios
    assert(*filePartner!=NULL); //Se tiver ocorrido algum erro e o ficheiro não tenha sido aberto, o programa termina

    fgets(fileName,MAXLINE,config);
    assert(fileName!=NULL); //No caso de o ficheiro não conter nenhuma linha, é feito um assert e o programa fecha
    fileName[strlen(fileName)-2] = '\0';
    *fileShares = fopen(fileName,"r"); //abertura do ficheiro que contém os valores já pagos pelos sócios
    assert(*fileShares!=NULL); //Se tiver ocorrido algum erro e o ficheiro não tenha sido aberto, o programa termina

    //É libertada a memória que foi alocada para guardar os dados na variável fileName
    free(fileName);
    fileName = NULL;

    //Os vários asserts nesta função devem-se ao facto de a abertura destes ficheiros ser imperativa para o continuamento
    //do programa
}

/*
 * ENTRADA: Função recebe ponteiro para um FILE *, binFile, que inicialmente se encontra a NULL, mas que guardará o
 * ficheiro aberto
 * SAIDA: Função não devolve parâmetro algum
 */
void openBinFile(FILE** binFile, char* text){
    //É alocada memória para as variáveis do tipo char*
    char* fileName = (char*)malloc(MAXLINE);
    char *filePath = (char*)malloc(MAXLINE);

    //A função corre até o ficheiro ser aberto com sucesso, pedindo ao utilizador qual o nome do ficheiro
    while(*binFile == NULL) {
        printf("%s: ",text);
        fgets(fileName,MAXLINE,stdin); //recebe da consola o nome do ficheiro
        if (*fileName=='\n'){ //Se o utilizador não estiver inserido um nome
            fprintf(stderr,"Invalid name\n"); //é enviada uma mensagem de erro para o stderr e volta-se ao início do ciclo
            continue;
        }
        //Se não tiver ocorrido nenhum erro até agora
        fileName[strlen(fileName)-1] = '\0'; //troca-se o \n do final da string por um '\0'
        sprintf(filePath,"%s%s.bin",PATH,fileName); //Monta-se o caminho para o ficheiro, unindo a PATH (valor global), com
        //o nome dado pelo utilizador e a extensão .bin
        *binFile = fopen(filePath, "wb"); //Abertura do ficheiro binário
        if (*binFile == NULL) //Verificação da abertura do ficheiro, no caso de erro, é enviada uma mensagem de erro
            fprintf(stderr,"Invalid name\n");
    }

    //É limpa toda a memória que foi alocada no início da função
    free(fileName);
    fileName = NULL;
    free(filePath);
    filePath = NULL;
    return;
}

/*
 * ENTRADA: Recebe o ficheiro config, aberto na main, de modo a lê-lo e abrir os restantes ficheiros
 * SAIDA: Funão não devolve nenhum valor
 */
void readData (FILE* config){

    //São inicializadas as variáveis, nomeadamente os dois ficheiros (com o valor NULL) e a linha que está a ser lida em cada iteração
    FILE* dataPartner = NULL;
    FILE* dataShares = NULL;
    int line = 0;

    //Inicialização das variáveis char* e double, temporárias, de modo a fazer a leitura dos ficheiros para as estruturas
    char* filePartnerLn = (char*) malloc(MAXLINE);
    char* fileShareLn = (char*) malloc(MAXLINE);
    char* name = (char*) malloc(MAXNAME);
    double shareValue = 0;

    //É chamada a função para abrir os ficheiros, a partir do config; para esta função são passados os endereços das
    //variáveis dataPartner e dataShares, inicializadas no início da função, de modo a que, após o openFiles terminar,
    //essas variáveis tenham o valor dos ficheiros abertos
    openFiles(config,&dataPartner,&dataShares);

    assert(fileCountLines(dataPartner)==fileCountLines(dataShares));

    //Inicialização das listas ligadas onde serão inseridos os os dados de sócios e seus valores já pagos
    initializeLists();

    //É lida cada uma das linhas de cada um dos ficheiros, até não haver mais nenhuma linha ara ler
    while (fgets(filePartnerLn,MAXLINE,dataPartner)!=NULL && fgets(fileShareLn,MAXLINE,dataShares)!=NULL){

        //filePartnerLn[strlen(filePartnerLn)-2]='\0'; //remove-se o \r\n do final da linha
        line++; //É incrementado o valor da linha (começa a 1)

        //testa-se o formato dos dados lidos do ficheiro; apenas se entra no if se ambos estiverem a true
        if (testPartnerInput(filePartnerLn,name,line) && testShareInput(fileShareLn,&shareValue,line)){
            addPartner(name); //O nome, depois de verificado o formato, é adicionado à lista ligada dataPartner
            addShare(shareValue); //O valor pago, depois de verificado o formato, é adicionado à lista ligada dataShares
        }
        //São limpados todos os dados dos char*, de modo a não haver sobreposição de dados
        memset(filePartnerLn,0,strlen(filePartnerLn));
        memset(fileShareLn,0,strlen(fileShareLn));
        memset(name,0,strlen(name));
    }

    //printShares(sharesLists);
    //printPartners(clientsList);

    //É limpa toda a memória alocada no início da função e fechados os ficheiros abertos
    free(filePartnerLn);
    filePartnerLn = NULL;

    free(fileShareLn);
    fileShareLn = NULL;

    free(name);
    name = NULL;


    fclose(dataPartner);
    fclose(dataShares);

}

/*
 * ENTRADA: Função não recebe qualquer parâmetro de entrada
 * SAIDA: Função não devolve nada
 */
void processData(void){

    //Declaração das variáveis
    

    //Inicialização das listas ligadas onde serão inseridos os dados de sócios com quotas pagas e em dívida
    initializeResList();

    //Inicialização de duas variáveis auxiliares, que contêm o valor da cabeça das listas ligadas
    
    addToLists();

    /*
    //DEBUG
    printf("#------------DATA PAID------------#\n");
    printPaid(paidList);
    printf("#------------DATA DUE------------#\n");
    printDue(dueList);
    */

}

/*
 * ENTRADA: Função não recebe qualquer parâmetro de entrada
 * SAIDA: Função não devolve nada
 */
void saveData (void){
    //Inicializadas as variáveis do tipo FILE* a NULL
    FILE *binPaidFile = NULL;
    FILE *binDueFile = NULL;

    //São abertos os ficheiros binários, necessários para guardar os dados dos sócios, recorrendo à função openBinFie
    //printf("Paid Partners\n");
    openBinFile(&binPaidFile,"Name of file to store data regarding the partners with paid payments");
    //printf("Due Partners\n");
    openBinFile(&binDueFile,"Name of file to store data regarding the partners with due payments");

    //São escritos os dados para os ficheiros binários respetivos
    writePaid(binPaidFile);
    writeDue(binDueFile);
	
	printf("Operation has been successfull\n");

    //É limpa toda a memória utilizada pelas várias listas ligadas
    destroyData();
}
//endregion
