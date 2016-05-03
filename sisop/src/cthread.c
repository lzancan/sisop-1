//------------------------------------------------------------------\\
//             Biblioteca cthread                                   \\
//               Instituto de Inform�tica - UFRGS                   \\
//                                                                  \\
//	              Sisop I                                       \\
//                                                                  \\
//	                Gabriel Ammes Pinho                         \\
//                      Luciano Zancan                              \\
//                                                                  \\
//------------------------------------------------------------------\\


//Includes e declara��es globais
#include "../include/cdata.h"
#include "../include/fila2.h"
#include "../include/cthread.h"



int ccreate (void* (*start)(void*), void *arg){ // cria nova thread
        return cria_thread (start, arg);
}

int cyield(void){ // libera cpu
        return liberando_cpu ();
}

int cjoin(int tid){ // sincroniza��o
        return sincronizacao (tid);
}

int csem_init(csem_t *sem, int count){ // inicia o sem�foro
        return inicia_semaforo (sem,count);
}

int cwait(csem_t *sem){ // aloca um recurso
        return aloca_recurso (sem);
}

int csignal(csem_t *sem){ // libera um recurso
        return libera_recurso (sem);
}

