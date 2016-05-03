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

int escalonador(){
	 if(em_execucao->state==executando){
	    em_execucao->state=apta;
	    getcontext(&(em_execucao->context));
	    insere_tcb(&fila_aptos,em_execucao);
        if(FirstFila2(&fila_aptos)==0){
                em_execucao=GetAtIteratorFila2(&fila_aptos);
                em_execucao->state=executando;
                setcontext(&(em_execucao->context));
                if(DeleteAtIteratorFila2(&fila_aptos))
                    return 0;
				//setcontext(&(em_execucao->context));//executa outra thread
        }
	 }

     return -1;

}


