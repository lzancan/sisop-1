#
# Makefile ESQUELETO
#
# OBRIGATÓRIO ter uma regra "all" para geração da biblioteca e de uma
# regra "clean" para remover todos os objetos gerados.
#
# NECESSARIO adaptar este esqueleto de makefile para suas necessidades.
#
# OBSERVAR que as variáveis de ambiente consideram que o Makefile está no diretótio "pithread"
# 

CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src
CXXFLAGS += -I$(INC_DIR)

all: libpithread.a

libpithread.a: pithread.o scheduler.o tcbtools.o
	@ar crs $(LIB_DIR)/libpithread.a $(BIN_DIR)/pithread.o $(BIN_DIR)/scheduler.o $(BIN_DIR)/tcbtools.o
	@echo "\nPithread compiled.\n"

pithread.o: $(SRC_DIR)/pithread.c 
	$(CC) -c $(CXXFLAGS) $^ -o $(BIN_DIR)/$@ -Wall

scheduler.o: $(SRC_DIR)/scheduler.c 
	$(CC) -c $(CXXFLAGS) $^ -o $(BIN_DIR)/$@ -Wall

tcbtools.o: $(SRC_DIR)/tcbtools.c 
	$(CC) -c $(CXXFLAGS) $^ -o $(BIN_DIR)/$@ -Wall

#regran: #dependências para a regran
#	$(CC) -o $(BIN_DIR)regran $(SRC_DIR)regran.c -Wall

clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~
