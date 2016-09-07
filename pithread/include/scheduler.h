#ifndef __scheduler__
#define __scheduler__

#include "pidata.h"
#include "enumthreads.h"
#include "tcbtools.h"
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>


#define SUCCESS_CODE 0
#define ERROR_CODE -1

//***************************************************************************
//************************ estruturas principais ****************************
//***************************************************************************

TCB_t* ReadyActiveQueue; //fila de aptos-ativos
TCB_t* ReadyExpiredQueue; //fila de aptos-expirados
TCB_t* BlockedQueue; //fila de bloqueados
TCB_t* BlockedWaitingQueue; //fila de bloqueados que estão esperando outra tcb terminar


//***************************************************************************
//****************************** funções ************************************
//***************************************************************************

/******************************************************************************
Função: Inicializa as estruturas de dados e contextos necessários

Saída:  0, se realizada com sucesso
        -1, se ocorreu algum erro
******************************************************************************/
int init_Structures();

/******************************************************************************
Função: Cria uma nova thread

Entra:  start_routine: ponteiro para a função que implementa a thread
        arg: ponteiro para os parâmetros que podem ser passados para a 
        thread, na sua criação.

Saída:  Retorna o identificador da thread criada, se for um valor positivo;
        Retorna -1 se ocorreu algum erro
******************************************************************************/
int createNewThread(int credCreate, void* (*start)(void*), void *arg );

/******************************************************************************
Função: Cria um bloco de controle de thread (Thread Control Block) 
	setando e atualizando o TID da mesma.

Saída:  Retorna um bloco de controle contendo todas as informações da thread
******************************************************************************/
TCB_t* createTCB(int creditos);

/******************************************************************************
Função: Cria a thread main

Saída:  Retorna o identificador da thread main (valor 0) em caso de sucesso;
        Retorna -1 se ocorreu algum erro
******************************************************************************/
int createMainThread(ucontext_t MainContext);

/******************************************************************************
Função: Faz a thread em execução liberar a cpu recalculando os creditos

Saída:  0, se realizada com sucesso
        -1, se ocorreu algum erro.
******************************************************************************/
int releaseCPU();

/******************************************************************************
Função: Bloqueia a thread em execução

Saída:  0, se realizada com sucesso
        -1, se ocorreu algum erro.
******************************************************************************/
int blockThread(int WaitingThreadID);

/******************************************************************************
Função: Remove uma thread da fila de bloqueados e a coloca na fila de aptos

Saida:  Retorna um ponteiro para a thread desbloqueada, caso alguma thread foi
        desbloqueada.
        Retorna NULL se nenhuma thread foi desbloqueada
******************************************************************************/
TCB_t* unblockThread();

/******************************************************************************
Função: Escalona as threads

Saída:  0, se realizada com sucesso
        -1, se ocorreu algum erro.
******************************************************************************/
int scheduler();

/******************************************************************************
Função: Encerra uma thread
******************************************************************************/
void exitThread();

//***************************************************************************
//*********************** estruturas de controle ****************************
//***************************************************************************
TCB_t* ExecutingThread; //indica a thread em execução
ucontext_t SchedulerContext; //contexto do escalonador
ucontext_t ExitContext; //contexto da função que encerra uma thread

/******************************************************************************
Função: Cria o contexto da função de escalonamento
******************************************************************************/
void createSchedulerContext();



/******************************************************************************
Função: Cria o contexto da função que encerra uma thread
******************************************************************************/
void createExitContext();

#endif
