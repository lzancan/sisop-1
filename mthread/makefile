CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src

all: pithreadFunctions.o pithread.o libpithread.a

# H1* = .h; C1* = .c
H1* = pithreadFunctions.h pidata.h
H1 = $(patsubst %,$(INC_DIR)/%,$(H1*))
C1* = pithreadFunctions.c
C1 = $(patsubst %,$(SRC_DIR)/%,$(C1*))
pithreadFunctions.o: $(C1) $(H1)
	$(CC) -o $(BIN_DIR)/pithreadFunctions.o -c $(SRC_DIR)/pithreadFunctions.c -I$(INC_DIR) -Wall

#H2* = .h; C2* = .c
H2* = pithread.h pithreadFunctions.h pidata.h
H2 = $(patsubst %,$(INC_DIR)/%,$(H2*))
C2* = pithread.c pithreadFunctions.c
C2 = $(patsubst %,$(SRC_DIR)/%,$(C2*))
pithread.o: $(C2) $(H2)
	$(CC) -o $(BIN_DIR)/pithread.o -c $(SRC_DIR)/pithread.c -I$(INC_DIR) -Wall

libpithread.a: pithread.o pithreadFunctions.o
	ar crs $(LIB_DIR)/libpithread.a $(BIN_DIR)/pithread.o $(BIN_DIR)/pithreadFunctions.o

clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~