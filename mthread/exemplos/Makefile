#
# Makefile para programas exemplo
# 

CC=gcc
LIB_DIR=../lib

all: exemplo mandel teste_vetor teste_prio test01

exemplo: exemplo.c $(LIB_DIR)/libpithread.a
	$(CC) -o exemplo exemplo.c -L$(LIB_DIR) -lpithread -Wall -g

teste_vetor: teste_vetor.c $(LIB_DIR)/libpithread.a
	$(CC) -o teste_vetor teste_vetor.c -L$(LIB_DIR) -lpithread -Wall

teste_prio: teste_prio.c $(LIB_DIR)/libpithread.a
	$(CC) -o teste_prio teste_prio.c -L$(LIB_DIR) -lpithread -Wall

mandel: mandel.c $(LIB_DIR)/libpithread.a
	$(CC) -o mandel mandel.c -L$(LIB_DIR) -lpithread -lm -Wall
	
test01: test01.c $(LIB_DIR)/libpithread.a
	$(CC) -o test01 test01.c -L$(LIB_DIR) -lpithread -lm -Wall

clean:
	rm -rf exemplo mandel teste_vetor teste_prio test01 *~




