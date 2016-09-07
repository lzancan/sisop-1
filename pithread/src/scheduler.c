#include "scheduler.h"



TCB_t* initQueue(){
	return NULL;
}

void insertZombie(TCB_t* zombieThread)
{
	zombieThread->next = BlockedWaitingQueue;
	BlockedWaitingQueue  = zombieThread;	
}

//Procura por uma TCB em todas as filas, últil para localizar a Thread que o Zombie está esperando;
TCB_t* searchForTCB(int ThreadID)
{
	TCB_t* Target;

	Target = searchTCB(ReadyActiveQueue, ThreadID);
	if (Target == NULL) Target = searchTCB(ReadyExpiredQueue, ThreadID);
	if (Target == NULL) Target = searchTCB(BlockedQueue, ThreadID);
	return Target;
}

//Inicializa todas as estruturas básicas
int initStructures()
{
	int ReturnCode; //cod de retorno da funcao
//--------------Inicializa as filas---------------------
	ReadyActiveQueue = initQueue();
	ReadyExpiredQueue = initQueue();
	BlockedQueue = initQueue();
	BlockedWaitingQueue = initQueue();
	
	ExecutingThread = NULL;//inicialmente nao ha nenhuma thread em execucao

	TID_Counter = 0; //A primeira thread a ser criada recebe zero(main)

//---------------inicializacao de contextos-----------------------------

	createSchedulerContext();//contexto do escalonador
	createExitContext();//contexto da funcao que encerra uma thread

//---------------cria thread main-----------------------
	ucontext_t MainContext;
	ReturnCode = getcontext(&MainContext);//pega contexto da main
	if(ReturnCode == SUCCESS_CODE)
	{
		ReturnCode = createMainThread(MainContext);//cria a thread main
		return(ReturnCode);
	}
	return(ERROR_CODE);
}

int createNewThread(int credCreate, void* (*start)(void*), void *arg)
{
	TCB_t* NewTCB = createTCB(credCreate);//aloca memoria para o TCB e tambem ja calcula o TID e atualiza o proprio
	if(NewTCB != NULL)
	{
		getcontext(&(NewTCB->context));//cria um molde(necessario para o makecontext)
		NewTCB->context.uc_link = &ExitContext;//contexto de saida para encerrar as threads
		makecontext(&(NewTCB->context),(void(*)(void))start,1,arg);//cria o contexto para a nova thread
		insertTCB(ReadyActiveQueue,NewTCB); //Insere novo TCB na afila de aptos;
		return NewTCB->tid;//retorna o TID da thread criada
	}
	return ERROR_CODE;
}

//----------------------------transformando a main em uma thread----------------------------

int createMainThread(ucontext_t MainContext)
{
	TCB_t* NewTCB = createTCB(100);//aloca memoria pro TCB e seta o TID
	
	if(NewTCB != NULL)//se nao houve erro
	{
		NewTCB->context = MainContext;//contexto salvo dentro da funcao init_Structures()
		NewTCB->context.uc_link = NULL;//encerra a thread
		NewTCB->state = Running; //indica que a main ja esta executando
		ExecutingThread = NewTCB;//indica que a main ja esta em execucao
		if(NewTCB->tid == 0)//a main tem que ter o TID zero.
		{
			return NewTCB->tid;
		}
	}
	return ERROR_CODE;
}

//-------------------------Libera a cpu------------------------------------------------------
int releaseCPU()
{
	ExecutingThread->credReal = ExecutingThread->credReal - 10;//perde 10 creditos por liberar a CPU
	ExecutingThread->state = Ready;
	
	return scheduler(0);//chama o escalonador
}

//------------------------------bloqueia a thread-------------------------------------------
int blockThread(int WaitingThreadID)//WaitingThreadID eh o TID da thread que a thread atual vai esperar
{
	ExecutingThread->state = Zombie;// seta estado como bloqueado
	return scheduler(WaitingThreadID);
}

//-----------------------------desbloqueia a thread -----------------------------------------
TCB_t* unblockThread()
{
	/*
	TCB_t* BlockThread;

	//TODO verifica se existe alguma thread bloqueada esperando pela thread atual(esse eh um exemplo de chamada de funcao que poderia ser)
	//BlockedThread = removeFromBlockedQueue(ExecutingThread->tid);//passa como parametro o TID da thread que esta em execucao e vai terminar
	
	if(BlockedThread != NULL)
	{
		//TODO remove nodo da fila de bloqueados por waiting
		BlockedThread->credReal = BlockedThread->credReal+20;//recebe 20 creditos por sair do estado de bloqueado(definicao do trablaho)
		if(BlockedThread->credReal > 100)
			BlockedThread->credReal = 100;//nenhuma thread pode ter mais do que 100 creditos(Definicao do trabalho)
		if(BlockedThread->tid != 0)//se nao for a main
		{
			if(BlockedThread->credReal > 0)
			{
				BlockedThread->state = Ready;
				//TODO coloca a thread desbloqueada na fila de aptos ativos
			}
			else
			{
				BlockedThread->state = Ready;
				BlockedThread->credReal = BlockedThread->credCreate;
				//TODO coloca a thread desbloqueada em aptos expirados
			}
		}
		return BlockedThread;
	}*/
	return NULL;//caso nenhuma thread seja desbloqueada
}

//*************************************************************************************************************
//******************************************** ESCALONADOR ****************************************************
//*************************************************************************************************************
int scheduler(int WaitingThreadID)
{
	int ReturnCode = SUCCESS_CODE; //cod de retorno
	volatile int ContextSwappFlag = 0;//indica se houve troca de contexto

	switch(ExecutingThread->state)
	{
		case Ready://em caso de chamada de yield
			if(ExecutingThread->credReal <= 0) //Testa se ainda possui créditos
			{
				//Se não tiver, reinicia seus créditos originais e coloca na fila de aptos-expirados
				ExecutingThread->credReal = ExecutingThread->credCreate; 
				insertTCB(ReadyExpiredQueue,ExecutingThread);
			}
			else
			{
				//Caso contrário coloca na fila de aptos-ativos
				insertTCB(ReadyActiveQueue,ExecutingThread);
			}
			ReturnCode = getcontext(&(ExecutingThread->context));//captura o contexto atual da thread
			break;

		case Blocked://em caso de chamada de lock
		insertTCB(BlockedQueue,ExecutingThread);
		ReturnCode = getcontext(&(ExecutingThread->context));
		break;

		case Zombie: //em caso de chamada de wait
			if(WaitingThreadID != 0) // TODO: E se tiver esperando pela Thread Main?
			{
				TCB_t* target = searchTCB(BlockedWaitingQueue, WaitingThreadID); // Testa se não há uma thread esperando por ela
				if(target == NULL)
				{
					target = searchForTCB(WaitingThreadID);
					ExecutingThread->prev = target;
					insertZombie(ExecutingThread);
				}
			}
			break;

			default:
			ReturnCode = ERROR_CODE;
			break;
		}

	//----------------ponto de retorno ao retomar contexto salvo acima------------

	if(ContextSwappFlag == 0) //se a thread ainda nao foi trocada
	{
		ContextSwappFlag = 1; //indica que a thread ja sofreu swapp
		//ExecutingThread = funcao que obtem da fila de aptos a proxima thread TODO
		

		//*************************************************************
		//************** PONTO DE POSSIVEL DEADLOCK *******************
		//*************************************************************

		if(ExecutingThread != NULL) //se havia uma thread na fila de aptos
		{
			if(ReturnCode == SUCCESS_CODE) // caso nao houve erro na captura de contexto acima
			{
				ExecutingThread->state = Running;//seta a nova thread atual como executando
				ReturnCode = setcontext(&(ExecutingThread->context));//executa outra thread
			}
		}
		else //nao havia uma thread na fila de aptos, entao sinaliza deadlock e encerra aplicacao
		{
			printf("\nDeadklock. Terminating application\n\n");
			exit(ERROR_CODE);//caso tenha ocorrido um deadlock
		}
	}
	return(ReturnCode);
}




void createSchedulerContext()
{
    getcontext(&SchedulerContext); //cria molde (necessário para a função makecontext)      
    SchedulerContext.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
    SchedulerContext.uc_stack.ss_size = SIGSTKSZ;
    SchedulerContext.uc_link = NULL;
    makecontext( &SchedulerContext, (void (*)(void))scheduler, 0);
}

//***************************************************************************************************
//****************** função que cria o contexto da função que encerra uma thread ********************
//***************************************************************************************************
void createExitContext()
{
    getcontext(&ExitContext); //cria molde (necessário para a função makecontext)
    ExitContext.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
    ExitContext.uc_stack.ss_size = SIGSTKSZ;
    ExitContext.uc_link = NULL;//&SchedulerContext; //após encerrar a thread, vai para o escalonador
    makecontext( &ExitContext, (void (*)(void))exitThread, 0);
}

//---------------------------funcao que encerra uma thread-------------------------------------
void exitThread()
{
        TCB_t* UnblockThread; //ponteiro para a thread desbloqueada

        UnblockThread = unblockThread(); //verifica se há alguma thread aguardando pela thread recem terminada
        ExecutingThread->state = Zombie; //coloca o estado da thread em execucao como Zombie

        if(UnblockThread != NULL && UnblockThread->tid == 0) //se for a thread main
        {
                //TODO move a thread em execucao para a fila de Zombies
                UnblockThread->state = Running; //indica que a thread main esta em execucao
                ExecutingThread = UnblockThread;
                setcontext(&(UnblockThread->context));//vai para o contexto da main
        }
}
