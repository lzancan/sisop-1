#include "../include/pithreadFunctions.h"


// ID das threads criadas
int threadID;

// Variaveis de controle de Fila de Aptos
int readyActive = 0;
int readyExpired = 1;

// Thread em execução
TCB_t *threadRunning = NULL;
TCB_t *threadMain;

// Contextos importantes
ucontext_t contextTerminator;
ucontext_t contextDispatcher;

// Fila de mutex
pimutexList_t *queueMutex = NULL;

// Filas de Aptos(Ativos e Expirados) e fila de bloqueados em wait
TCB_t *queueReadyActive = NULL;
TCB_t *queueReadyExpired = NULL;

TCBwait_t *queueBlockedWait = NULL;

// Cria primeira instância da main_thread, dispatcher e terminator
void createBasicThreads(void){
    // Cria o contexto Main
    threadID = 0;
    ucontext_t contextMain;
    if (getcontext(&contextMain) == -1) handle_error("getcontext");

    // Cria a threadMain
    threadMain = (TCB_t*)malloc(sizeof(TCB_t)); // alocação de espaço para o bloco TCB
    threadMain->tid = threadID++;
    threadMain->credCreate = 100;
    threadMain->credReal = 100;
    threadMain->context = contextMain;
    threadMain->next = NULL;
    threadMain->prev = NULL;
    threadMain->state = STATE_RUNNING;
    threadRunning = threadMain;
    
    // Cria o contexto Terminator
    if (getcontext(&contextTerminator) == -1) handle_error("getcontext");
    contextTerminator.uc_link = NULL;
    contextTerminator.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ); // Stack Pointer
    contextTerminator.uc_stack.ss_size = SIGSTKSZ; // Stack Size
    makecontext(&contextTerminator,terminator,0);

    // Cria o contexto Dispatcher
    if (getcontext(&contextDispatcher) == -1) handle_error("getcontext");
    contextDispatcher.uc_link = NULL;
    contextDispatcher.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ); // Stack Pointer
    contextDispatcher.uc_stack.ss_size = SIGSTKSZ; // Stack Size
    makecontext(&contextDispatcher,dispatcher,0);
}

// Fila de aptos - Inserção, remoção e busca de thread
void queueReadyInsert(TCB_t *tcbInsert, TCB_t **queue){
    TCB_t *tcbAnt, *tcbAtual;
    
    tcbAnt = NULL;
    tcbAtual = *queue;
    
        //enquanto existir alguem na fila e o elemento que queremos inserir tem os creditos maiores ou iguais ao elemento que estamos analisando, segue adiante
        while( tcbAtual != NULL && (tcbInsert->credReal <= tcbAtual->credReal))
        {
            tcbAnt = tcbAtual;
            tcbAtual = tcbAtual->next;
        }
        tcbInsert->next = tcbAtual;//encontrou a posicao correta pelo numero de creditos, insere o elemento.
        if(tcbAnt == NULL ) *queue = tcbInsert;
        else tcbAnt->next = tcbInsert;

    tcbInsert->state = STATE_READY; // Muda do estado Criacao para o estado Apto
}

bool queueReadyFind(int tid){
    TCB_t *tcbNow = queueReadyActive;

    while(tcbNow != NULL){ // Se a fila está vazia, prossegue para a fila seguinte
        if(tcbNow->tid == tid) // Se localizar o tid, interrompe a busca
            return true;
        tcbNow = tcbNow->next;
    }

 
    tcbNow = queueReadyExpired;
    while(tcbNow != NULL){ // Se a fila está vazia, prossegue para a fila seguinte
        if(tcbNow->tid == tid) // Se localizar o tid, interrompe a busca
            return true;
        tcbNow = tcbNow->next;
    }

    return false;
}

TCB_t* queueReadyRemove(){
    TCB_t *tcbRemove = NULL;
    if(queueReadyActive|| queueReadyExpired)
    {
        if(queueReadyActive == NULL) // Se Ativo estiver vazio, inverte fila com o expirado
        {
            TCB_t* temp;
            temp = queueReadyActive;
            queueReadyActive = queueReadyExpired;
            queueReadyExpired = temp;
        }
        tcbRemove = queueReadyActive; //Remove o primeiro da fila de aptos-ativos
        queueReadyActive = queueReadyActive->next;
        tcbRemove->next = NULL;
        return tcbRemove;
    } 
    return tcbRemove;
}


// Fila de bloqueados em wait - Inserção, remoção e buscas de thread e de tid
void queueBlockedWaitInsert(TCBwait_t *tcbWaitInsert){
    tcbWaitInsert->tcbBlocked->state = STATE_BLOCKED;
    tcbWaitInsert->next = queueBlockedWait;
    queueBlockedWait = tcbWaitInsert;
}

bool queueBlockedWaitFindThread(int tid){
    TCBwait_t *tcbWaitNow = queueBlockedWait;
    while(tcbWaitNow != NULL){
        if(tcbWaitNow->tcbBlocked->tid == tid) // Se localizar o tid, interrompe a busca
            return true;
        tcbWaitNow = tcbWaitNow->next;}
    return false;
}

bool queueBlockedWaitFindTID(int tid){
    TCBwait_t *tcbWaitNow = queueBlockedWait;
    while(tcbWaitNow != NULL){
        if(tcbWaitNow->tidWaited == tid) // Se localizar o tid, interrompe a busca
            return true;
        tcbWaitNow = tcbWaitNow->next;}
    return false;
}

TCBwait_t* queueBlockedWaitRemove(int tid){
    if (queueBlockedWait == NULL){
        return NULL;
    } else{
        TCBwait_t *tcbWaitNow = queueBlockedWait;
        TCBwait_t *tcbWaitLast = queueBlockedWait;
        TCBwait_t *tcbWaitReturn = NULL;
        while (tcbWaitNow != NULL){
            if (tcbWaitNow->tidWaited == tid){
                if (tcbWaitNow == queueBlockedWait){ // Remoção do início da fila
                    queueBlockedWait = queueBlockedWait->next;
                } else{  // Remoção do ínicio da fila
                    tcbWaitLast->next = tcbWaitNow->next;}
                tcbWaitReturn = tcbWaitNow;
                tcbWaitReturn->next = NULL;
                return tcbWaitReturn;
            } else{ // Avança para a posição seguinte
                tcbWaitLast = tcbWaitNow;
                tcbWaitNow = tcbWaitNow->next;}}
        return tcbWaitReturn;}
}

// Fila de bloqueados em mutex - Inserção, remoção
void queueMutexTCBInsert(pimutex_t *mtx, TCB_t *tcbInsert)
{
    tcbInsert->state = STATE_BLOCKED;
    // Seta o estado da thread como bloqueada e insere no final da fila de bloqueados do mutex
    if(mtx->last == NULL)
    {
        mtx->last = tcbInsert;
        mtx->first = tcbInsert;
    }
    else
    {
        mtx->last->next = tcbInsert;
        mtx->last = tcbInsert;
    }
}

TCB_t* queueMutexTCBRemove(pimutex_t *mtx)
{
    TCB_t *tcbRemove = NULL;
    // Remove o primeiro da fila de bloqueados do mutex
    if(mtx->first != NULL)
    {
        tcbRemove = mtx->first;
        mtx->first = mtx->first->next;
        tcbRemove->next = NULL;
        
    }
    return tcbRemove;
}

// Fila de mutex - Inserção, busca de thread
void queueMutexInsert(pimutexList_t *mtxList)
{
    mtxList->next = queueMutex;
    queueMutex = mtxList;
}

bool queueMutexFindThread(int tid)
{
    pimutexList_t *mtxList = queueMutex;
    while(mtxList != NULL)
    { // Busca todos os mutex
        while(mtxList->mtx->first != NULL)
        { // Busca todas as threads bloqueadas pelo mutex
            if(mtxList->mtx->first->tid == tid)  // Se localizar o tid, interrompe a busca
                return true;
            mtxList->mtx->first = mtxList->mtx->first->next;
            
        }
        mtxList = mtxList->next;
        
    }
    return false;
}

// Dispatcher e finalizador de threads
void dispatcher()
{
    threadRunning = queueReadyRemove();
    
    if(threadRunning != NULL)
    {
        // Se havia thread apta, executa seu contexto da thread
        threadRunning->state = STATE_RUNNING;
        if(setcontext(&(threadRunning->context)) == -1) handle_error("setcontext");
        
    }

    // Encerramento com erro se nenhuma thread estiver apta para executar
    exit(255);
}

void terminator(){
    if(threadRunning->tid != 0)
    {
        // Se houver thread bloqueada esperando, libera para que volte aos aptos
        TCBwait_t *tcbWait = queueBlockedWaitRemove(threadRunning->tid);
        if(tcbWait){
            addCredit(tcbWait->tcbBlocked);//adiciona 20 creditos para a thread que saiu de bloqueado e foi para aptos
            queueReadyInsert(tcbWait->tcbBlocked, &queueReadyActive);
            free(tcbWait);}

        // Libera e memória de stack e do bloco
        free(threadRunning->context.uc_stack.ss_sp);
        free(threadRunning);}

    // Volta para o dispatcher
    if(setcontext(&contextDispatcher) == -1) handle_error("setcontext");
}

void addCredit(TCB_t* tcbTarget)
{
    tcbTarget->credReal = tcbTarget->credReal + 20;
    if(tcbTarget->credReal > 100) tcbTarget->credReal = 100;
}

int removeCredit(TCB_t* tcbTarget)
{
    tcbTarget->credReal = tcbTarget->credReal - 10;
    return tcbTarget->credReal;
}

//Lista todos os elementos da fila
void printNodes(TCB_t* Queue)
{
    TCB_t* Temp; //nodo auxiliar para percorrer a fila
    if(Queue == NULL)
        printf("Fila vazia");
    else
	{
		Temp = Queue;
		//printf("\n--------------\n");
		while(Temp != NULL)
		{
			printf("\nTID: %d CredReal: %d CredCreate: %d \n",
			        Temp->tid, Temp->credReal, Temp->credCreate);
			Temp = Temp->next;
		}
	   // printf("--------------\n");
	}            
}