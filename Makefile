COMPILER = gcc
OPTIONS = -c -g -Wall

all: cliente servidor

servidor: tp_socket.o common.o servidor.o
	$(COMPILER) tp_socket.o common.o servidor.o -o servidor

cliente: tp_socket.o common.o cliente.o
	$(COMPILER) tp_socket.o common.o cliente.o -o cliente

servidor.o: servidor.c
	$(COMPILER) $(OPTIONS) servidor.c

cliente.o: cliente.c
	$(COMPILER) $(OPTIONS) cliente.c

common.o: common.c
	$(COMPILER) $(OPTIONS) common.c

tp_socket.o: tp_socket.c
	$(COMPILER) $(OPTIONS) tp_socket.c

clean:
	rm -f *.o cliente servidor janela tp_socket
