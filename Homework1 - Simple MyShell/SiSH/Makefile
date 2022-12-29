CC = gcc -O2
OBJS = built.o func.o main.o read.o
TARGET = SiSH

clean:
	rm -f *.o
	rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

func.o: func.c
read.o: read.c
built.o: built.c
main.o: SiSH.h main.c

