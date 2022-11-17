CC = gcc -O2 -Wall
OBJS = main.o heap.o queue.o msg.o u_signal.o
TARGET = main

all : $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)


$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)


main.o: msg.h heap.h queue.h main.c
heap.o: queue.h heap.h heap.c
queue.o: queue.h queue.c
msg.o: queue.h msg.h msg.c
u_signal.o: msg.h queue.h heap.h u_signal.h u_signal.c
