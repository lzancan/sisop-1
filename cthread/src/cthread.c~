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


//Includes e declarações globais
#include "cdata.h"
#include "fila2.h"
#include "cthread.h"



int ccreate (void* (*start)(void*), void *arg){ // cria nova thread
        return cria_thread (start, arg);
}

int cyield(void){ // libera cpu
        return liberando_cpu ();
}

int cjoin(int tid){ // sincronização
        return sincronizacao (tid);
}

int csem_init(csem_t *sem, int count){ // inicia o semáforo
        return inicia_semaforo (sem,count);
}

int cwait(csem_t *sem){ // aloca um recurso
        return aloca_recurso (sem);
}

int csignal(csem_t *sem){ // libera um recurso
        return libera_recurso (sem);
}

