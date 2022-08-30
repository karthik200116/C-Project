a.out: client.o server.o
		gcc server.o client.o -lm

server.o: server.c interface.h
		gcc -c server.c -lm

client.o: client.c interface.h
		gcc -c client.c -lm