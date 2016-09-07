#include "tcbtools.h"

//Insere TCB na fila
void insertTCB(TCB_t* Queue, TCB_t* newTCB){

	TCB_t* Cursor;

	if(Queue == NULL){
		Queue = newTCB;
	} else {
		Cursor = Queue;
		while(Cursor != NULL){
			if(newTCB->credReal > Cursor->credReal){//Se o elemento do cursor tiver um crédito menor que o do novo elemento insere na lista
				newTCB->prev = Cursor->prev;
				newTCB->next = Cursor;
				Cursor->prev = newTCB;
				break;
			} else if (Cursor->next == NULL){ //Se for o último elemento, insere no fim
				Cursor->next = newTCB;
				newTCB->prev = Cursor;
			}
			Cursor = Cursor->next;
		}
	}
}

//Procura por uma TCB na Fila
TCB_t* searchTCB(TCB_t* Queue, int ThreadID){
	TCB_t* Cursor;
	Cursor = Queue;
	if(Queue != NULL)
	{
		while((Cursor != NULL) && (Cursor->tid != ThreadID))
			Cursor = Cursor->next;
	} 
	
	return Cursor; //Se a fila estiver vazia ou não encotrar a TID, retornará NULL
}


//------------------cria um TCB --------------------------
TCB_t* createTCB(int credCreate)
{
	TCB_t* NewTCB = NULL;
	if(credCreate>=1 && credCreate<=100)
	{
		NewTCB = (TCB_t*)malloc(sizeof(TCB_t));//aloca memoria e cria uma nova thread
	
		if(NewTCB != NULL)
		{
			NewTCB->tid = TID_Counter;//seta o TID
			NewTCB->state = NewThread;//seta o estado inicial da thread
			TID_Counter++;//atualiza o contador de TID para a proxima thread
	
			NewTCB->state = Ready;
			NewTCB->credCreate = credCreate;//creditos de criacao da thread. Minimo 1, Maximo 100
			NewTCB->credReal = credCreate;//credito atuais da thread. Minimo 1, Maximo 100
				
			//alocando estrutura pro controle de contexto
			NewTCB->context.uc_stack.ss_sp = (char*)malloc(SIGSTKSZ*sizeof(char));
			NewTCB->context.uc_stack.ss_size = (SIGSTKSZ*sizeof(char));
			//o campo UC_LINK eh setado na funcao de criacao da thread
		}
	}
	return NewTCB;
}
