DIR_INC = ./ipc
DIR_OBJ = ./obj
DIR_SRC = ./src
DIR_BIN = ./bin
CFLAGS = -I$(DIR_INC)
LINK = $(DIR_INC)/ipc.o

.PHONY: all clean
all: producer smoker

producer: chkObjDir chkBinDir $(LINK) $(DIR_OBJ)/producer.o
	gcc -o $(DIR_BIN)/producer $(LINK) $(DIR_OBJ)/producer.o $(CFLAGS)

smoker: chkObjDir chkBinDir $(LINK) $(DIR_OBJ)/smoker.o
	gcc -o $(DIR_BIN)/smoker $(LINK) $(DIR_OBJ)/smoker.o $(CFLAGS)

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	gcc -c $< -o $@ $(CFLAGS)

$(DIR_INC)/%.o: $(DIR_INC)/%.c
	gcc -c $< -o $@ $(CFLAGS)

chkObjDir:
	@if test ! -d $(DIR_OBJ); \
	then \
		mkdir $(DIR_OBJ); \
	fi

chkBinDir:
	@if test ! -d $(DIR_BIN); \
	then \
		mkdir $(DIR_BIN); \
	fi

clean: 
	-rm -rf $(DIR_OBJ) $(DIR_BIN)