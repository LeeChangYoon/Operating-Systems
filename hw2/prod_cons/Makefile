CC = gcc -O2 -pthread 
TARGET = prod_cons_v1 prod_cons_v2.1 prod_cons_v2.2 prod_cons_v2.3 prod_cons_v3 prod_cons_v4
OBJS =  prod_cons_v1.o prod_cons_v2.1.o prod_cons_v2.2.o prod_cons_v2.3.o prod_cons_v3.o prod_cons_v4.o

clean:
	rm -f *.o
	rm -f $(TARGET)


prod_cons: $(OBJS)
	$(CC) prod_cons_v1.o -o prod_cons_v1
	$(CC) prod_cons_v2.1.o -o prod_cons_v2.1
	$(CC) prod_cons_v2.2.o -o prod_cons_v2.2
	$(CC) prod_cons_v2.3.o -o prod_cons_v2.3
	$(CC) prod_cons_v3.o -o prod_cons_v3
	$(CC) prod_cons_v4.o -o prod_cons_v4
	rm -f *.o


prod_cons_v1.o: prod_cons_v1.c
prod_cons_v2.1.o: prod_cons_v2.1.c
prod_cons_v2.2.o: prod_cons_v2.2.c
prod_cons_v2.3.o: ring_buffer.h prod_cons_v2.3.c
prod_cons_v3.o: prod_cons_v3.c
prod_cons_v4.o: prod_cons_v4.c
