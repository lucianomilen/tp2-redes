/*==================================================================================================================
Trabalho Prático 2 - Sockets UDP, janela deslizante                                 
Redes de Computadores                               
Aluno: Guilherme Saulo Alves               
time.h: Contém as declarações das funções e bibliotecas utilizadas em time.c
====================================================================================================================*/

#ifndef TIME_H
#define TIME_H

#include "janela.h"
#include <sys/time.h>
	
//Obtem um certo tempo do relogio do computador e retorna em segundos
double obtemTempo();

//Define o tempo timeout
void temporizador(int segundos);

#endif