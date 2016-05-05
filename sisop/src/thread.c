//------------------------------------------------------------------\\
//             Biblioteca cthread                                   \\
//               Instituto de Informática - UFRGS                   \\
//                                                                  \\
//	              Sisop I                                       \\
//                                                                  \\
//	                Gabriel Ammes Pinho                         \\
//                      Luciano Zancan                              \\
//                                                                  \\
//------------------------------------------------------------------\\



#include "../include/fila2.h"
#include "../include/cthread.h"
#include "../include/thread.h"
#include "../include/tcb.h"
#include <ucontext.h>



int inicializacao_estruturas()
{
    int cria_filas;
    if(CreateFila2(&fila_aptos)) // inicializa as filas
	if(CreateFila2(&fila_bloqueados)){} // do nothing


	TID=0; // thread main recebe 0


	cria_contexto_escalonador();//contexto do escalonador

	ucontext_t contexto_main;
	int flag_contexto = getcontext(&contexto_main);
	if(flag_contexto == 0){
		return cria_thread_main(contexto_main);
	}
	else return -1;

}

int cria_thread_main(ucontext_t contexto_main)
{
	TCB_t* tcb = cria_tcb();
	if(tcb != NULL){
		tcb->state =executando;
		tcb->context = contexto_main;
		tcb->context.uc_link = NULL;
		em_execucao = *tcb;
		if(tcb->tid == 0){
			return tcb->tid;
		}
	}
	else return -1;
	return -1;
}


int cria_thread (void* (*start)(void*), void *arg){
    TCB_t* tcb = cria_tcb();
	if(tcb == NULL){
	        return -1;
    }
	else{
		getcontext(&(tcb->context));
		makecontext(&(tcb->context),(void(*)(void))start,1,arg);
		insere_tcb(&fila_aptos,tcb);
		return tcb->tid;
	}
	return 0;
}


int liberando_cpu (){
    em_execucao.state=apta;
    return escalonador(0);
}


int sincronizacao (int tid){
    em_execucao.state=bloqueada;
    return escalonador(tid);
    return 0;
}


int inicia_semaforo (csem_t *sem, int count){
    return 0;
}



int aloca_recurso (csem_t *sem){
    return 0;
}


int libera_recurso (csem_t *sem){
    return 0;
}

