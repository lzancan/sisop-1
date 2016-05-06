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
#include <ucontext.h>



int cria_thread (void* (*start)(void*), void *arg){
        TCB_t* tcb = cria_tcb();
        if(tcb == NULL){
                return -1;
        }
        else{
            getcontext(&(tcb->context));
            makecontext(&(tcb->context),(void(*)(void))start,1,arg);
            if(tcb->state==criada) tcb->state = apta;
            if(TID>0){
                insere_tcb(&fila_aptos,tcb);
            }
            return tcb->tid;
        }
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
    sem->count=count;
    sem->fila=&fila_bloqueados;
    if(sem!=NULL) return 0;
    else return 1;
}



int aloca_recurso (csem_t *sem){
    if(sem->count >0){
        sem->count=sem->count-1;
        return 0;
    }
    else{
       sem->count=sem->count-1;
       em_execucao.state=bloqueada;
       return escalonador(-1);
    }
}


int libera_recurso (csem_t *sem){
    sem->count=sem->count+1;
    return desbloqueia_recurso();
}

