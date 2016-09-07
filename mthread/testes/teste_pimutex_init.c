#include "../include/pithread.h"
#include <stdio.h>
#include <stdlib.h>

pimutex_t* pi;


int main(void){
    printf("Iniciando operacao.\n");
    
    if((pi = (pimutex_t *)malloc(sizeof(pimutex_t)))==NULL)
        return -1;
        
    if(pimutex_init(pi)==-1){
        printf("Erro pimutex_init.\n");
        exit(200);
    }
    printf("Flag: %d\nInicio Lista: %p\nFim Lista: %p\n", pi->flag, pi->first, pi->last);

    printf("\nFlag tem que ser sempre inicializada como 1(que significa livre)\nEncerrando main.\n");
    return 0;
}
