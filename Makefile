CC=gcc
CFLAGS=-g -I.
DEPS=Ftp.h hashtable.h
OBJ=Ftp.o Commands.o Hashtable.o Structs.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ftp: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean: 
	rm *.o ftp
