COMPILER = gcc
OPTIONS = -c -g -Wall

all: cliente servidor

servidor: tp_socket.o janela.o io.o time.o servidor.o
	$(COMPILER) tp_socket.o janela.o io.o time.o servidor.o -o servidor

cliente: tp_socket.o janela.o io.o time.o cliente.o
	$(COMPILER) tp_socket.o janela.o io.o time.o cliente.o -o cliente

servidor.o: servidor.c
	$(COMPILER) $(OPTIONS) servidor.c

cliente.o: cliente.c
	$(COMPILER) $(OPTIONS) cliente.c

io.o: io.c
	$(COMPILER) $(OPTIONS) io.c

time.o: time.c
	$(COMPILER) $(OPTIONS) time.c

janela.o: janela.c
	$(COMPILER) $(OPTIONS) janela.c

tp_socket.o: tp_socket.c
	$(COMPILER) $(OPTIONS) tp_socket.c

clean:
	rm -f *.o cliente servidor janela io time tp_socket