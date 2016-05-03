#include "../include/tcb.h"
#include "../include/fila2.h"




TCB_t* cria_tcb (){
        TCB_t* tcb = (TCB_t*)malloc(sizeof(TCB_t));
        if(tcb != NULL){
                tcb->tid=TID;
                tcb->state=criada;
                TID++;
                tcb->context.uc_stack.ss_sp = (char*)malloc(SIGSTKSZ*sizeof(char));
	        tcb->context.uc_stack.ss_size = (SIGSTKSZ*sizeof(char));

        }
        return tcb;
}

void insere_tcb(PFILA2 fila,TCB_t* tcb_nova){
        if(AppendFila2(fila, tcb_nova)){}
}




void cria_contexto_escalonador()
{
    getcontext(&contexto_escalonador);
    contexto_escalonador.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
    contexto_escalonador.uc_stack.ss_size = SIGSTKSZ;
    contexto_escalonador.uc_link = NULL;
    makecontext( &contexto_escalonador, (void (*)(void))escalonador, 0);
}

int escalonador(int TID)
{
	volatile int trocou_de_contexto = 0;

	if(em_execucao->state==apta){
	    insere_tcb(fila_aptos,em_execucao);
	    if(trocou_de_contexto==0) trocou_de_contexto=1;
        return getcontext(&(em_execucao->context));
	}
	else if (em_execucao->state == bloqueada){
		//insere_tcb(fila_bloqueados,em_execucao);
		if(trocou_de_contexto==0) trocou_de_contexto=1;
		return getcontext(&(em_execucao->context));
	}
	else return -1;

}


