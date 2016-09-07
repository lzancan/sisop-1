/*
 * mdata.h: arquivo de inclus�o de uso apenas na gera��o da libmthread
 *
 * Esse arquivo pode ser modificado. ENTRETANTO, deve ser utilizada a TCB fornecida.
 *
 */
#ifndef __pidata__
#define __pidata__

#include <ucontext.h>

// Estrutura das threads
typedef struct TCB {
	int  		tid;		// identificador da thread
	int  		state;	   	// estado em que a thread se encontra
    						// 0: Criação; 1: Apto; 2: Execução; 3: Bloqueado e 4: Término
	int 		credCreate;	// créditos atribuídos na criação (correspondente à prioridade inicial)
	int 		credReal;	//créditos atuais da thread (usado para determinar a prioridade atual da thread)
	ucontext_t 	context;	// contexto de execu��o da thread (SP, PC, GPRs e recursos)
	struct TCB 	*prev;		// ponteiro para o TCB anterior da lista
	struct TCB 	*next;		// ponteiro para o pr�ximo TCB da lista
} TCB_t;

// Estrutura dos blocos de espera
typedef struct TCBwait{
    TCB_t *tcbBlocked;
    int tidWaited;
    struct TCBwait *next;
} TCBwait_t;

// Declaração de booleano
typedef int bool;
#define true 1
#define false 0

#endif
