#include <stdio.h>
#include <stdlib.h>
#include "../include/cdata.h"
#include "../include/fila2.h"
#include "../include/thread.h"
#include <ucontext.h>


int TID;


TCB_t* cria_tcb ();

void insere_tcb(PFILA2 pFila,TCB_t* tcb_nova);
int escalonador(int tid);
TCB_t* procura_tcb (PFILA2 fila, int tid);
