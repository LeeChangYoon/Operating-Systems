CC = gcc -O2
TARGET = prod_cons_v1 prod_cons_v2.1 prod_cons_v2.2 prod_cons_v3 prod_cons_v4


clean:
	rm -f *.o
	rm -f $(TARGET)


prod_cons: 
	$(CC) -pthread prod_cons_v1.c -o prod_cons_v1
	$(CC) -pthread prod_cons_v2.1.c -o prod_cons_v2.1
	$(CC) -pthread prod_cons_v2.2.c -o prod_cons_v2.2
	$(CC) -pthread prod_cons_v3.c -o prod_cons_v3
	$(CC) -pthread prod_cons_v4.c -o prod_cons_v4

