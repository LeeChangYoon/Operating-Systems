CC=gcc
CFLAGS=-g -Wall
OBJS=main.o hash_set.o file_system.o SiSH/SiSH.o SiSH/parser.o SiSH/list.o SiSH/execute.o SiSH/keyboard.o SiSH/console.o SiSH/base.o SiSH/error.o
TARGET=main

all : $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

clean :
	rm *.o main

main.o: SiSH/SiSH.h file_system.h fs.h main.c
hash_set.o: hash_set.h hash_set.c
file_system.o: hash_set.h file_system.h file_system.c
SiSH/SiSH.o: file_system.h SiSH/color.h SiSH/execute.h SiSH/base.h SiSH/SiSH.h SiSH/SiSH.c
SiSH/parser.o: SiSH/parser.h SiSH/base.h SiSH/list.h SiSH/parser.c
SiSH/list.o: SiSH/list.h SiSH/list.c
SiSH/execute.o : SiSH/execute.h SiSH/base.h SiSH/parser.h SiSH/execute.c
SiSH/keyboard.o: SiSH/keyboard.h SiSH/keyboard.c
SiSH/console.o: SiSH/console.h SiSH/console.c
SiSH/base.o: SiSH/base.h SiSH/base.c
SiSH/error.o: SiSH/error.h SiSH/error.c