DIR_OBJ = ./obj
DIR_SRC = ./src
DIR_BIN = ./bin

SRC = $(wildcard $(DIR_SRC)/*.c)
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))

CFLAGS = -lpthread
TARGET = pmain tmain

.PHONY: all clean
all: $(TARGET)

pmain: $(DIR_OBJ)/pmain.o
	gcc -o $(DIR_BIN)/$@ $< $(CFLAGS)

tmain: $(DIR_OBJ)/tmain.o
	gcc -o $(DIR_BIN)/$@ $< $(CFLAGS)

$(DIR_OBJ)/pmain.o: $(DIR_SRC)/pmain.c
	gcc -c $< -o $@ $(CFLAGS)

$(DIR_OBJ)/tmain.o: $(DIR_SRC)/tmain.c
	gcc -c $< -o $@ $(CFLAGS)

clean:
	-rm $(DIR_BIN)/pmain $(DIR_BIN)/tmain $(OBJ) 