#ifndef __THREAD_H__
#define __THREAD_H__


// includes

#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "../include/cdata.h"
#include "../include/fila2.h"
#include "../include/cthread.h"
#include "../include/tcb.h"


enum estados_das_threads {
        criada=0,
        apta=1,
        executando=2,
        bloqueada=3,
        terminada=4
        };

FILA2 fila_aptos;
FILA2 fila_bloqueados;

TCB_t em_execucao; //thread que está sendo executada

ucontext_t contexto_escalonador; // contexto do escalonador
ucontext_t contexto_main;

void cria_contexto_escalonador();

int escalonador(int tid);


int cria_thread_main(ucontext_t contexto_main);

int cria_thread (void* (*start)(void*), void *arg);

int liberando_cpu ();


int sincronizacao (int tid);


int inicia_semaforo (csem_t *sem, int count);



int aloca_recurso (csem_t *sem);


int libera_recurso (csem_t *sem);

#endif
