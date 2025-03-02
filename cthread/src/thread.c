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



#include "cthread.h"
#include "thread.h"




int cria_thread (void* (*start)(void*), void *arg,PFILA2 fila_aptos){
        TCB_t* tcb = cria_tcb();
	if(tcb == NULL)
	        return -1;
	else{
		getcontext(&(tcb->context));
		tcb->context.uc_link = &ExitContext;
		makecontext(&(tcb->context),(void(*)(void))start,arg);
		insere_tcb(fila_aptos,tcb); 
		return tcb->tid;
	}
}


int liberando_cpu (){

}


int sincronizacao (int tid){

}


int inicia_semaforo (csem_t *sem, int count){

}



int aloca_recurso (csem_t *sem){

}


int libera_recurso (csem_t *sem){

}

