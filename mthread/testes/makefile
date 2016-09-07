CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src

all: teste_create_yield teste_pilock_piunlock teste_pimutex_init teste_wait teste_ativo_expirado

teste_create_yield: teste_create_yield.c
	$(CC) -o teste_create_yield teste_create_yield.c -L.$(LIB_DIR) -lpithread -Wall

teste_pilock_piunlock: teste_pilock_piunlock.c
	$(CC) -o teste_pilock_piunlock teste_pilock_piunlock.c -L.$(LIB_DIR) -lpithread -Wall

teste_pimutex_init: teste_pimutex_init.c
	$(CC) -o teste_pimutex_init teste_pimutex_init.c -L.$(LIB_DIR) -lpithread -Wall

teste_wait: teste_wait.c
	$(CC) -o teste_wait teste_wait.c -L.$(LIB_DIR) -lpithread -Wall
	
teste_ativo_expirado: teste_ativo_expirado.c
	$(CC) -o teste_ativo_expirado teste_ativo_expirado.c -L.$(LIB_DIR) -lpithread -Wall

clean:
	rm -rf *.o teste_create_yield teste_pilock_piunlock teste_pimutex_init teste_wait teste_ativo_expirado  *~
