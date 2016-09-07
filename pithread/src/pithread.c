//*******************************************************************
//                                                                  *
//              Universidade Federal do Rio Grande do Sul           *
//	                 Instituto de Informática                       *
//                                                                  *
//	                 Sistemas Operacionais I                        *
//                                                                  *
//               pithread - Gerenciador de threads N:1              *
//                                                                  *
//	                    Augusto Ferreira                            *
//                     Rodrigo Haas de Araujo Rech                  *
//                                                                  *
//*******************************************************************

//**************************** Includes *****************************

#include "pithread.h"
#include "scheduler.h"

//*******************************************************************
//**************************** Funções ******************************
//*******************************************************************


//************************ picreate ***********************
int picreate (int credCreate, void* (*start)(void*), void *arg)
{
	return createNewThread(credCreate, start, arg);
}

//**************************** piyield ******************************
int piyield(void)
{
	return releaseCPU();
}

//***************************** uth_wait ****************************
int piwait(int tid)
{
	return blockThread(tid);
}

//***************************** pimutex_init ************************
int pimutex_init(pimutex_t *mtx)
{
	return -1;
}

//***************************** pilock ******************************
int pilock(pimutex_t *mtx)
{
	return -1;
}

//***************************** piunlock ****************************
int piunlock(pimutex_t *mtx)
{
	return -1;
}
