DIR_OBJ = ./obj
DIR_SRC = ./src
DIR_BIN = ./bin

SRC = $(wildcard $(DIR_SRC)/*.c)
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))

CFLAGS = 
TARGET = main

.PHONY: clean all
all: $(TARGET)

$(TARGET): $(OBJ)
	gcc -o $(DIR_BIN)/$@ $^ $(CFLAGS)

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	gcc -c $< -o $@ $(CFLAGS)

clean:
	-rm $(DIR_BIN)/* $(OBJ)