DIR_OBJ = ./obj
DIR_SRC = ./src
DIR_BIN = ./bin

SRC = $(wildcard $(DIR_SRC)/*.c)
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))

CFLAGS = -lpthread
TARGET = pmain tmain

.PHONY: all clean
all: $(TARGET)

pmain: chkObjDir chkBinDir $(DIR_OBJ)/pmain.o
	gcc -o $(DIR_BIN)/$@ $(DIR_OBJ)/pmain.o $(CFLAGS)

tmain: chkObjDir chkBinDir $(DIR_OBJ)/tmain.o
	gcc -o $(DIR_BIN)/$@ $(DIR_OBJ)/tmain.o $(CFLAGS)

$(DIR_OBJ)/pmain.o: $(DIR_SRC)/pmain.c
	gcc -c $< -o $@ $(CFLAGS)

$(DIR_OBJ)/tmain.o: $(DIR_SRC)/tmain.c
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