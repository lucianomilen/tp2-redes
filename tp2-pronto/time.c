/*==================================================================================================================
Trabalho Prático 2 - Sockets UDP, janela deslizante                        
Redes de Computadores                               
Aluno: Guilherme Saulo Alves               
time.c: Define a função gettimeofday
====================================================================================================================*/

#include "time.h"

double obtemTempo(){
	
	struct timeval time;
	gettimeofday(&time, NULL);

	return time.tv_sec + (time.tv_usec/1000000.0);
}

void temporizador(int segundos){

    struct timeval tv;
    tv.tv_sec=segundos;
    tv.tv_usec=0;

    setsockopt(idsocket_,SOL_SOCKET,SO_RCVTIMEO,(struct timeval*)&tv, sizeof(struct timeval));
}