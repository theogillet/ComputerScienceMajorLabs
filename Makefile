CC = gcc
FLAGS = -Wall

gettftp : createRRQ.o createACK.o recupServer.o gettftp.o
	      $(CC) createRRQ.o createACK.o recupServer.o gettftp.o -g -o gettftp

gettftp.o : gettftp.h gettftp.c
		   $(CC) -c gettftp.c $(FLAGS) -g -o gettftp.o

puttftp : createWRQ.o createACK.o recupServer.o puttftp.o
	      $(CC) createWRQ.o createACK.o recupServer.o puttftp.o -g -o puttftp

puttftp.o : puttftp.h puttftp.c
		   $(CC) -c puttftp.c $(FLAGS) -g -o puttftp.o

createRRQ.o : createRRQ.h createRRQ.c
		   $(CC) -c createRRQ.c $(FLAGS) -g -o createRRQ.o
		   
createWRQ.o : createWRQ.h createWRQ.c
		   $(CC) -c createWRQ.c $(FLAGS) -g -o createWRQ.o
		
createACK.o : createACK.h createACK.c
		   $(CC) -c createACK.c $(FLAGS) -g -o createACK.o
		   
recupServer.o : recupServer.h recupServer.c
		   $(CC) -c recupServer.c $(FLAGS) -g -o recupServer.o

clean :
		rm -f *.o
		rm -f gettftp
		rm -f puttftp
		
