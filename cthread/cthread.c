#include "../include/fila2.h"
#include "../include/cthread.h"
#include <stdlib.h>
#include <stdio.h>

int ccreate (void *(*start)(void *), void *arg);
Par�metros:
start: ponteiro para a fun��o que a thread executar�.
arg: um par�metro que pode ser passado para a thread na sua cria��o. (Obs.: � um �nico par�metro. Se for necess�rio passar mais de um valor deve-se empregar um ponteiro para uma struct)
Retorno:
Quando executada corretamente: retorna um valor positivo, que representa o identificador da thread criada
Caso contr�rio, retorna um valor negativo.

int cyield(void);
Retorno:
Quando executada corretamente: retorna 0 (zero)
Caso contr�rio, retorna um valor negativo.

int cjoin(int tid);
Par�metros:
tid: identificador da thread cujo t�rmino est� sendo aguardado.
Retorno:
Quando executada corretamente: retorna 0 (zero)
Caso contr�rio, retorna um valor negativo.

int csem_init (csem_t *sem);
Par�metros:
sem: ponteiro para uma vari�vel do tipo csem_t. Aponta para uma estrutura de dados que representa a vari�vel sem�foro.
Retorno:
Quando executada corretamente: retorna 0 (zero)
Caso contr�rio, retorna um valor negativo.

int cwait (csem_t *sem);
Par�metros:
sem: ponteiro para uma vari�vel do tipo sem�foro.
Retorno:
Quando executada corretamente: retorna 0 (zero)
Caso contr�rio, retorna um valor negativo.

int csignal (csem_t *sem);
Par�metros:
sem: ponteiro para uma vari�vel do tipo sem�foro.
Retorno:
Quando executada corretamente: retorna 0 (zero)
Caso contr�rio, retorna um valor negativo.

