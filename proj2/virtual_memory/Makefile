CC = gcc -O2 -Wall
OBJS = main.o heap.o queue.o msg.o signal.o memory.o
TARGET = main

all : $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)


$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) -lm


queue.o: queue.h queue.c
msg.o: queue.h msg.h msg.c
memory.o: memory.h memory.c
heap.o: queue.h heap.h heap.c
main.o: msg.h heap.h queue.h main.c
signal.o: memory.h msg.h queue.h heap.h signal.h signal.c
