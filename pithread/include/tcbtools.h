#include "pidata.h"
#include "enumthreads.h"
#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS_CODE 0
#define ERROR_CODE -1

int TID_Counter; //controla o id da última thread criada

//Insere TCB na fila
void insertTCB(TCB_t* Queue, TCB_t* newTCB);

//Procura por uma TCB na Fila
TCB_t* searchTCB(TCB_t* Queue, int ThreadID);

//Procura por uma TCB em todas as filas, últil para localizar a Thread que o Zombie está esperando;
TCB_t* searchForTCB(int ThreadID);

//------------------cria um TCB --------------------------
TCB_t* createTCB(int credCreate);