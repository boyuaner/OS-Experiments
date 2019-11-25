DIR_INC = ../ipc
DIR_OBJ = ./obj
DIR_SRC = ./src
DIR_BIN = ./bin

SRC = $(wildcard $(DIR_SRC)/*.cc)
OBJ = $(patsubst %.cc,${DIR_OBJ}/%.o,$(notdir ${SRC}))

CFLAGS = -I$(DIR_INC) -lstdc++
LINK = $(DIR_INC)/ipc.o

TARGET = main

.PHONY: all clean
all: $(TARGET)

$(TARGET): chkObjDir chkBinDir $(LINK) $(OBJ)
	gcc -o $(DIR_BIN)/$@ $(LINK) $(OBJ) $(CFLAGS)

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.cc
	gcc -c $< -o $@ $(CFLAGS)

$(DIR_INC)/%.o: $(DIR_INC)/%.cc
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