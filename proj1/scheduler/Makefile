CC = gcc -O2
OBJS = main.o
TARGET = main


clean:
	rm -f *.o
	rm -f $(TARGET)


$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)


main.o: msg.h heap.h queue.h main.c
