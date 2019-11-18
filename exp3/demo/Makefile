DIR_OBJ = ./obj
DIR_SRC = ./src
DIR_BIN = ./bin

SRC = $(wildcard $(DIR_SRC)/*.c)
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))

CFLAGS = 
TARGET = psched

.PHONY: all clean
all: $(TARGET)

$(TARGET): chkObjDir chkBinDir $(OBJ)
	gcc -o $(DIR_BIN)/$@ $(OBJ) $(CFLAGS)

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
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
	-rm -rf $(DIR_BIN) $(DIR_OBJ)