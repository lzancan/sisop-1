#
# Makefile ESQUELETO
#
# OBRIGATÓRIO ter uma regra "all" para geração da biblioteca e de uma
# regra "clean" para remover todos os objetos gerados.
#
# NECESSARIO adaptar este esqueleto de makefile para suas necessidades.
#  1. Cuidado com a regra "clean" para não apagar o "fila2.o"
#
# OBSERVAR que as variáveis de ambiente consideram que o Makefile está no diretótio "cthread"
# 

CC=gcc
LIB_DIR=./lib/
INC_DIR=./include/
BIN_DIR=./bin/
SRC_DIR=./src/

all: cthread

cthread: thread.o fila2.o
	$(CC) -o $(BIN_DIR)cthread $(SRC_DIR)thread.c -Wall
thread.o: $(SRC_DIR)thread.c
	gcc -c $(SRC_DIR)thread.c -Wall
fila2.o: $(SRC_DIR)fila2.c
	gcc -c $(SRC_DIR)fila2.c -Wall
clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~


