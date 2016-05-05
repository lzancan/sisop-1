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
        if(AppendFila2(fila, (TCB_t*)tcb_nova)){}
}




void cria_contexto_escalonador()
{
    getcontext(&contexto_escalonador);
    contexto_escalonador.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
    contexto_escalonador.uc_stack.ss_size = SIGSTKSZ;
    contexto_escalonador.uc_link = NULL;
    makecontext( &contexto_escalonador, (void (*)(void))escalonador, 0);
}

int escalonador(int tid){
     TCB_t* tcb_aux =NULL;
     tcb_aux=&em_execucao;
	 if(em_execucao.state==apta){
	    getcontext(&(em_execucao.context));
	    insere_tcb(&fila_aptos,&em_execucao);
        if(FirstFila2(&fila_aptos)==0){
                tcb_aux=(TCB_t*)GetAtIteratorFila2(&fila_aptos);
                em_execucao=*tcb_aux;
                em_execucao.state=bloqueada;
                setcontext(&(em_execucao.context));
                if(DeleteAtIteratorFila2(&fila_aptos))
                    return 0;
        }
	 }
	 if(em_execucao.state==bloqueada){
        if(AppendFila2(&fila_bloqueados, tcb_aux)==0){
            getcontext(&(em_execucao.context));
            tcb_aux=procura_tcb(&fila_aptos,tid);
            em_execucao=*tcb_aux;
            em_execucao.state=executando;
            setcontext(&(em_execucao.context));
        }
	 }

     return -1;
}

TCB_t* procura_tcb (PFILA2 fila, int tid){

    TCB_t* tcb=NULL;
    int achou=0;
    int tid_procurado=-1;
    if(FirstFila2(fila)==0){
        tcb=GetAtIteratorFila2(fila);
        tid_procurado=tcb->tid;
        if(tid_procurado==tid) achou = 1;
        else{
            achou = 0;
            while(!achou && fila->it!=fila->last){
                fila->it = fila->it->next;
                tcb=GetAtIteratorFila2(fila);
                tid_procurado=tcb->tid;
                if(tid_procurado==tid) achou = 1;
                else achou = 0;
            }

        }
    }
    return tcb;

}


