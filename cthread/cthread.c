#include "../include/fila2.h"
#include "../include/cthread.h"
#include <stdlib.h>
#include <stdio.h>

int ccreate (void *(*start)(void *), void *arg);
Parâmetros:
start: ponteiro para a função que a thread executará.
arg: um parâmetro que pode ser passado para a thread na sua criação. (Obs.: é um único parâmetro. Se for necessário passar mais de um valor deve-se empregar um ponteiro para uma struct)
Retorno:
Quando executada corretamente: retorna um valor positivo, que representa o identificador da thread criada
Caso contrário, retorna um valor negativo.

int cyield(void);
Retorno:
Quando executada corretamente: retorna 0 (zero)
Caso contrário, retorna um valor negativo.

int cjoin(int tid);
Parâmetros:
tid: identificador da thread cujo término está sendo aguardado.
Retorno:
Quando executada corretamente: retorna 0 (zero)
Caso contrário, retorna um valor negativo.

int csem_init (csem_t *sem);
Parâmetros:
sem: ponteiro para uma variável do tipo csem_t. Aponta para uma estrutura de dados que representa a variável semáforo.
Retorno:
Quando executada corretamente: retorna 0 (zero)
Caso contrário, retorna um valor negativo.

int cwait (csem_t *sem);
Parâmetros:
sem: ponteiro para uma variável do tipo semáforo.
Retorno:
Quando executada corretamente: retorna 0 (zero)
Caso contrário, retorna um valor negativo.

int csignal (csem_t *sem);
Parâmetros:
sem: ponteiro para uma variável do tipo semáforo.
Retorno:
Quando executada corretamente: retorna 0 (zero)
Caso contrário, retorna um valor negativo.

