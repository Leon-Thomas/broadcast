CC = gcc
CFLAGS = -lpthread -lList
HOME = ..
HEADERS = -I$(HOME)/headers
LIBS	= -L$(HOME)/lib

install:server client
	rm *.o
client: client_2.c
	$(CC) -o $@ client_2.c $(LIBS) $(HEADERS)
server:	michat.o
	$(CC) -o $@ michat.o $(LIBS) $(CFLAGS) 
michat.o:	michat.c
	$(CC) -c -o $@ michat.c $(HEADERS) 
clean:
	-rm server client
