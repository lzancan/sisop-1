//testa o esgotamento de credito das threads, fazendo que expirado vire ativo e verificando a ordem de ganho de cpu.

#include	"../include/pithread.h"
#include	<stdio.h>
#include	<stdlib.h>

void* f1(){
    int i;
    for(i=1; i<=5; i++){
        printf("\nthreadF1(%d)",i);
        piyield();
    }
    return NULL;
}

void* f2(){
    int i;
    for(i=1; i<=5; i++){
        printf("\nthreadF2(%d)",i);
        piyield();
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int id1;
    printf("Iniciando operacao.\n");
    id1 = picreate(20, f1, (NULL));
    picreate(30, f2, (NULL));
    printf("\nthread criada!.\n\n");
    piwait(id1);

    printf("\nConcluido main.\n");
    exit(0);
}