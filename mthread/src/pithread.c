// Biblioteca de Threads
// Augusto Cezar Muxfeldt Ferreira
// Rodrigo Haas de Araujo Rech
#include "../include/pithread.h"
#include "../include/pithreadFunctions.h"
#include "../include/pidata.h"


int picreate (int credCreate, void* (*start)(void*), void *arg)
{
    // Se não há threads em execução, cria a threadMain e os contextos do Terminator e do Dispatcher
    if (threadRunning == NULL)
    {
        createBasicThreads();
    }

    // Cria e configura o Thread Control Block
	TCB_t *tcbNew = (TCB_t*)malloc(sizeof(TCB_t)); // alocação de espaço para o bloco TCB
	if(tcbNew != NULL){
	    // Cria o contexto da Thread
        if (getcontext(&(tcbNew->context)) == -1) handle_error("getcontext");
        tcbNew->context.uc_link = &contextTerminator; // Contexto executado no encerramento da thread
        tcbNew->context.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ); // Stack Pointer
        tcbNew->context.uc_stack.ss_size = SIGSTKSZ; // Stack Size
        makecontext(&(tcbNew->context),(void (*)(void))start,1,arg);

		tcbNew->tid = threadID++;
		tcbNew->state = STATE_CREATION;
		tcbNew->credCreate = credCreate;
		tcbNew->credReal = credCreate;
		tcbNew->next = NULL;
		tcbNew->prev = NULL;
        queueReadyInsert(tcbNew,&queueReadyActive);
        return tcbNew->tid;}
    return -1;
}

int piyield(void){
    if (threadRunning == NULL)
        return -1;

    // Recoloca a thread nas listas de aptos
    if(removeCredit(threadRunning) <= 0)
    {
        threadRunning->credReal = threadRunning->credCreate;
        queueReadyInsert(threadRunning, &queueReadyExpired);
    }
    else
    {
        queueReadyInsert(threadRunning, &queueReadyActive);
    }
    // Troca o contexto para o dispatcher escolher o processo que será executado
    if (swapcontext(&(threadRunning->context),&contextDispatcher) == -1) handle_error("swapcontext");
    return 0;
}

int piwait(int tid){
    if (threadRunning == NULL)
        return -1;

    // Busca se há thread com o tid procurado na lista de aptos ou lista de bloqueados
    if(queueReadyFind(tid) ||  queueBlockedWaitFindThread(tid) || queueMutexFindThread(tid)){
        // Busca se a thread já é aguardada por outra thread
        if(!queueBlockedWaitFindTID(tid)){
            // Cria e configura o bloco de espera
            TCBwait_t *tcbWaitNew = (TCBwait_t*)malloc(sizeof(TCBwait_t));
            tcbWaitNew->tcbBlocked = threadRunning;
            tcbWaitNew->tidWaited = tid;
            queueBlockedWaitInsert(tcbWaitNew);
            removeCredit(threadRunning);
            // Troca o contexto para o dispatcher escolher o processo que será executado
            if (swapcontext(&(threadRunning->context),&contextDispatcher) == -1) handle_error("swapcontext");}
        return 0;
    } else{
        return -1;}
}

int pimutex_init(pimutex_t *mtx){
    if (threadRunning == NULL){
        createBasicThreads();}

    // Aloca o espaço para o mutex
    //mtx = (pimutex_t*)malloc(sizeof(pimutex_t));
    //mtx = (pimutex_t *)malloc(sizeof(pimutex_t));
    if(mtx != NULL){
        mtx->flag = STATE_FREE;//inicializa como livre
        mtx->first = NULL;
        mtx->last = NULL;
    } else{
        return -1;}

    // Aloca o espaço para o nodo da lista de mutex
    pimutexList_t *mtxList = (pimutexList_t*)malloc(sizeof(pimutexList_t));
    if(mtxList != NULL){
        mtxList->mtx = mtx;
        queueMutexInsert(mtxList);
        return 0;}
    return -1;
}

int pilock (pimutex_t *mtx){
    if (threadRunning == NULL){
        createBasicThreads();}

    // Testa se o mutex existe
    //printf("\n\nteste01\n\n");
    if(mtx != NULL){
        //printf("\n\nteste02\n\n");
        if(mtx->flag == STATE_BUSY)//secao critica esta ocupada
        {
            //remove os 10 creditos pela thread estar perdendo a cpu
            removeCredit(threadRunning);
            //coloca a thread em espera no final da fila
            
            queueMutexTCBInsert(mtx,threadRunning);

            // Troca o contexto para o dispatcher escolher o processo que será executado
            if (swapcontext(&(threadRunning->context),&contextDispatcher) == -1) handle_error("swapcontext");
            
        }
        else
        {//se a secao critica nao estava ocupada
            mtx->flag = STATE_BUSY;//seta a flag como ocupada
        }
        return 0;
        
    }
    return -1;
}

int piunlock (pimutex_t *mtx){
    if (threadRunning == NULL){
        createBasicThreads();}

    // Testa se o mutex existe
   // printf("\n\n unlock3 \n\n");
    if(mtx != NULL){
        //printf("\n\n  unlock4 \n\n");
        //printf("Flag %d", mtx->flag);
        if(mtx->flag == STATE_BUSY)//testa se a secao critica esta ocupada
        {
            //printf("\n\n unlock5 \n\n");
            // atualiza a flag para liberada
            mtx->flag = STATE_FREE;

            // Libera a primeira thread bloqueada pelo mutex
            TCB_t *tcbInsert = queueMutexTCBRemove(mtx);
            if(tcbInsert != NULL)
            {
                //printf("\n\n unlock6 \n\n");
                addCredit(tcbInsert);
                mtx->flag = STATE_BUSY;
                queueReadyInsert(tcbInsert,&queueReadyActive);//insere a threal removida de bloqueados para a fila de aptos ativos
            }
            return 0;
        }
        
    }
    return -1;
}
