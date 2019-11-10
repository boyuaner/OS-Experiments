DIR_INC = ../ipc
DIR_OBJ = ./obj
DIR_SRC = ./src
DIR_BIN = ./bin
CFLAGS = -I$(DIR_INC)
LINK = $(DIR_INC)/ipc.o

.PHONY: all clean
all: producer consumer

producer: $(LINK) $(DIR_OBJ)/producer.o
	gcc $(CFLAGS) -o $(DIR_BIN)/producer $(LINK) $(DIR_OBJ)/producer.o

consumer: $(LINK) $(DIR_OBJ)/consumer.o
	gcc $(CFLAGS) -o $(DIR_BIN)/consumer $(LINK) $(DIR_OBJ)/consumer.o

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	gcc $(CFLAGS) -c $< -o $@

$(DIR_INC)/%.o: $(DIR_INC)/%.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	-rm $(DIR_OBJ)/*.o $(DIR_BIN)/producer $(DIR_BIN)/consumer