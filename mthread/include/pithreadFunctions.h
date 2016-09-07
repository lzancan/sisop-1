#ifndef __pithreadFunctions__
#define __pithreadFunctions__

#include "pidata.h"
#include "pithread.h"
#include <stdio.h>
#include <stdlib.h>

// Tratamento de erros
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

// Estados poss�veis para as threads
#define STATE_CREATION 0
#define STATE_READY    1
#define STATE_RUNNING  2
#define STATE_BLOCKED  3
#define STATE_EXITING  4

// Estados poss�veis para os mutex
#define STATE_FREE 1
#define STATE_BUSY 0

// N�veis de prioridade da lista de aptos
#define readyQueues 2

// ID das threads criadas
extern int threadID;

// Variaveis de controle de Fila de Aptos
extern int readyActive;
extern int readyExpired;

// Thread em execu��o
extern TCB_t *threadRunning;
extern TCB_t *threadMain;

// Contextos importantes
extern ucontext_t contextTerminator;
extern ucontext_t contextDispatcher;

// Estrutura da lista de mutex
typedef struct mutexList {
    pimutex_t *mtx;
    struct mutexList *next;
} pimutexList_t;

// Fila de mutex
extern pimutexList_t *queueMutex;

// Filas de Aptos (ativos e expirados) e fila de bloqueados em wait
extern TCB_t *queueReadyActive;
extern TCB_t *queueReadyExpired;
extern TCBwait_t *queueBlockedWait;

// Cria primeira instancia da main_thread, dispatcher e terminator
void createBasicThreads(void);

// Fila de aptos - Insercao, busca de thread, remocao
void queueReadyInsert(TCB_t *tcbInsert, TCB_t **queue);
bool queueReadyFind(int tid);
TCB_t* queueReadyRemove();

// Fila de bloqueados em wait - Insercao, buscas de thread e de tid, remocao
void queueBlockedWaitInsert(TCBwait_t *tcbWaitInsert);
bool queueBlockedWaitFindThread(int tid);
bool queueBlockedWaitFindTID(int tid);
TCBwait_t* queueBlockedWaitRemove(int tid);

// Fila de bloqueados em mutex - Insercaoo, remocaoo
void queueMutexTCBInsert(pimutex_t *mtx, TCB_t *tcbInsert);
TCB_t* queueMutexTCBRemove(pimutex_t *mtx);

// Fila de mutex - Insercaoo, busca de thread
void queueMutexInsert(pimutexList_t *mtxList);
bool queueMutexFindThread(int tid);

// Dispatcher e finalizador de threads
void dispatcher();
void terminator();

//Controle de creditos
//funcao que adiciona os 20 creditos para a thread quando ela sai de bloqueado para aptos
void addCredit(TCB_t* tcbTarget);

//funcao que remove os 10 creditos quando a thread perde a cpu e retorna o valor atual de creditos
//que sendo negativos, necessitam de tratamento.
int removeCredit(TCB_t* tcbTarget);

void printNodes(TCB_t* Queue);

//que 
#endif
