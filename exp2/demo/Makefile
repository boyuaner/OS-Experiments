DIR_OBJ = ./obj
DIR_SRC = ./src
DIR_BIN = ./bin

SRC = $(wildcard $(DIR_SRC)/*.c)
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))

CFLAGS = -lpthread
TARGET = ppipe tpipe

.PHONY: all clean
all: $(TARGET) 

ppipe: chkObjDir chkBinDir $(DIR_OBJ)/ppipe.o
	gcc -o $(DIR_BIN)/$@ $(DIR_OBJ)/ppipe.o $(CFLAGS)

tpipe: chkObjDir chkBinDir $(DIR_OBJ)/tpipe.o
	gcc -o $(DIR_BIN)/$@ $(DIR_OBJ)/tpipe.o $(CFLAGS)

$(DIR_OBJ)/ppipe.o: $(DIR_SRC)/ppipe.c
	gcc -c $< -o $@ $(CFLAGS)

$(DIR_OBJ)/tpipe.o: $(DIR_SRC)/tpipe.c
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