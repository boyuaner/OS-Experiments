DIR_OBJ = ./obj
DIR_SRC = ./src
DIR_BIN = ./bin

SRC = $(wildcard $(DIR_SRC)/*.c)
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))

CFLAGS = -lpthread
TARGET = ppipe tpipe

.PHONY: all clean
all: $(TARGET) 

ppipe: $(DIR_OBJ)/ppipe.o
	gcc -o $(DIR_BIN)/$@ $< $(CFLAGS)

tpipe: $(DIR_OBJ)/tpipe.o
	gcc -o $(DIR_BIN)/$@ $< $(CFLAGS)

$(DIR_OBJ)/ppipe.o: $(DIR_SRC)/ppipe.c
	gcc -c $< -o $@ $(CFLAGS)

$(DIR_OBJ)/tpipe.o: $(DIR_SRC)/tpipe.c
	gcc -c $< -o $@ $(CFLAGS)

clean:
	-rm $(DIR_BIN)/ppipe $(DIR_BIN)/tpipe $(OBJ)