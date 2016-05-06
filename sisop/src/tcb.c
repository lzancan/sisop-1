#include "../include/tcb.h"
#include "../include/fila2.h"




TCB_t* cria_tcb (){
        TCB_t* tcb = (TCB_t*)malloc(sizeof(TCB_t));
        if(tcb != NULL){
            if(TID>0){
                tcb->tid=TID;
                tcb->state=criada;
                TID++;
                tcb->context.uc_stack.ss_sp = (char*)malloc(SIGSTKSZ*sizeof(char));
                tcb->context.uc_stack.ss_size = (SIGSTKSZ*sizeof(char));
            }
            else if (TID ==0)
            {
                tcb->state =executando;
                tcb->context = contexto_main;
                tcb->tid=0;
                tcb->context.uc_link = NULL;
                tcb->context.uc_stack.ss_sp = (char*)malloc(SIGSTKSZ*sizeof(char));
                tcb->context.uc_stack.ss_size = (SIGSTKSZ*sizeof(char));
                em_execucao = *tcb;
                TID++;
            }
        }
        return tcb;
}


void insere_tcb(PFILA2 fila,TCB_t* tcb_nova){
        if(AppendFila2(fila, (TCB_t*)tcb_nova)){}
}





int escalonador(int tid){
     TCB_t* tcb_aux=&em_execucao;
     if(em_execucao.state==bloqueada && tid==-1){
         getcontext(&(em_execucao.context));
         if(AppendFila2(&fila_bloqueados, tcb_aux)==0){
             if(FirstFila2(&fila_bloqueados)==0){
                tcb_aux=(TCB_t*)GetAtIteratorFila2(&fila_aptos);
                em_execucao=*tcb_aux;
                em_execucao.state=executando;
                setcontext(&(em_execucao.context));
             }
         }
     }
    else{
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
            getcontext(&(em_execucao.context));
            insere_tcb(&fila_bloqueados,&em_execucao);
            tcb_aux=procura_tcb(&fila_aptos,tid);
            em_execucao=*tcb_aux;
            em_execucao.state=executando;
            setcontext(&(em_execucao.context));
            printf("\n**ok2**\n");
            LastFila2(&fila_aptos);
            tcb_aux=(TCB_t*)GetAtIteratorFila2(&fila_bloqueados);
            tcb_aux->state=apta;
            DeleteAtIteratorFila2(&fila_bloqueados);
            AppendFila2(&fila_aptos, tcb_aux);
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
                if(NextFila2(fila)==0){
                    tcb=GetAtIteratorFila2(fila);
                    tid_procurado=tcb->tid;
                    if(tid_procurado==tid) achou = 1;
                    else achou = 0;
                }
            }

        }
    }
    return tcb;
}

int desbloqueia_recurso(void){
    TCB_t* tcb_aux=NULL;
    if(FirstFila2(&fila_bloqueados)==0){
        tcb_aux=GetAtIteratorFila2(&fila_bloqueados);
        tcb_aux->state=apta;
        if(DeleteAtIteratorFila2(&fila_bloqueados)&&AppendFila2(&fila_aptos, tcb_aux)==0){
            return 0;
        }


    }
    return -1;
}


