//Teste de picreate e piyield e dos creditos de prioridade das threads
//Teste faz a main ir perdendo creditos por chamadas de yield até ficar equivalente as threads criadas, e percebe-se o correto funcionamento do escalonador
//que escolhe as threads de acordo com seus creditos atuais.

#include	"../include/pithread.h"
#include	<stdio.h>
#include	<stdlib.h>

void* f1(){
    printf("\nFuncao F1 em execucao.\n\nF1 chamando yield!\n");
    piyield();
    printf("\nFuncao F1 encerrando.\n\n");
    return NULL;
}

void* f2(){
    printf("\nFuncao F2 em execucao.\n");
    printf("\nFuncao F2 encerrando.\n\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    printf("Iniciando operacao.\n");
    printf("Criacao da thread PID = %d\n",picreate(50, f1, (NULL)));
    printf("Criacao da thread PID = %d\n",picreate(50, f2, (NULL)));

    printf("\nmain com 90 creditos");
    piyield();
    printf("\nmain com 80 creditos");
    piyield();
    printf("\nmain com 70 creditos");
    piyield();
    printf("\nmain com 60 creditos");
    piyield();
    printf("\nmain com 50 creditos");
    piyield();

    printf("\nMain cedendo a CPU novamente.\n\n");
    piyield();

    printf("\nConcluido main.\n");
    exit(0);
}
