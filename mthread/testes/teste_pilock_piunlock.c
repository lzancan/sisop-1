#include "../include/pithread.h"
#include <stdio.h>
#include <stdlib.h>

pimutex_t pi;

void* f1(){
    printf("Entrando na f1\n");
    if(pilock(&pi)==-1){
        printf("Erro no mlock f1\n");
        exit(255);
    }

    printf("Area protegida f1\nFazendo yield para testar\n");
    piyield();

    if(piunlock(&pi)==-1){
        printf("Erro no munlock f1\n");
        exit(255);
    }
    printf("Saindo da f1\n");
    return NULL;
}

void* f2(){
    printf("Entrando na f2\n");
    if(pilock(&pi)==-1){
        printf("Erro no mlock f2\n");
        exit(255);
    }

    printf("Area protegida f2\n");

    if(piunlock(&pi)==-1){
        printf("Erro no munlock f2\n");
        exit(255);
    }
    printf("Saindo da f2\n");
    return NULL;
}


int main(void){
    int a;
    printf("Iniciando operacao.\n");

    
    if(pimutex_init(&pi)==-1){
        printf("Erro pimutex_init.\n");
        exit(200);
    }

    a = picreate(90,f1,(NULL));
    picreate(90,f2,(NULL));

    piwait(a);

    printf("Encerrando main.\n");
    return 0;
}
