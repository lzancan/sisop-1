//Teste de piwait e prioridades
#include	"../include/pithread.h"
#include	<stdio.h>
#include	<stdlib.h>
int id1,id2,id3,id4,id5,id6;

void* f1(){
    printf("Thread1 em execucao.\n\n");
    printf("Thread1 cedendo CPU.\n\n");
    piyield();
    printf("Thread1 encerrando.\n\n");
    return NULL;
}

void* f2(){
    printf("Thread2 em execucao.\n");
    printf("Thread2 encerrando.\n");
    return NULL;
}

void* f3(){
    printf("Thread3 em execucao.\n\n");
    printf("Thread3 vai tentar aguardar a Thread4.\n(mas thread 4 ja encerrou, entao thread 3 deve encerrar no prox passo!\n");
    piwait(id4); //Thread ja foi encerrada
    printf("Thread3 encerrando.\n\n");
    return NULL;
}

void* f4(){
    printf("Thread4 em execucao.\n\n");
    printf("Thread4 vai tentar aguardar a Thread2.\n(thread main ja esta aguardando, entao nao pode conseguir aguardar tambem)\n");
    piwait(id2); //Main ja esta esperando, entao a thread nao sera bloqueada
    printf("Thread4 encerrando.\n\n");
    return NULL;
}

void* f5(){
    printf("Thread5 em execucao.\n\n");
    printf("Thread5 cedendo CPU.\n\n");
    piyield();
    printf("Thread5 encerrando.\n\n");
    return NULL;
}

void* f6(){
    printf("Thread6 em execucao.\n\n");
    printf("Thread6 aguardando Thread5.\n\n");
    piwait(id5);
    printf("Thread6 encerrando.\n\n");
    return NULL;
}


int main(int argc, char *argv[]) {

    printf("Iniciando operacao.\n");

    
    id1 = picreate(10, f1, (NULL));
    id2 = picreate(20, f2, (NULL));
    id3 = picreate(10, f3, (NULL));
    id4 = picreate(30, f4, (NULL));
    id5 = picreate(20, f5, (NULL));
    id6 = picreate(30, f6, (NULL));
    
    printf("ThreadMain aguardando Thread2.\n\n");
    piwait(id2);

    printf("ThreadMain aguardando Thread1.\n\n");
    piwait(id1);

    printf("ThreadMain encerrando.\n\n TIDs: %d %d %d %d %d %d",id1,id2,id3,id4,id5,id6);
    exit(0);
}
