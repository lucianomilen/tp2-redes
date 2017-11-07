/*==================================================================================================================
Trabalho Prático 2 - Sockets UDP, janela deslizante                     
Redes de Computadores                               
Aluno: Guilherme Saulo Alves                         
io.h: Contém as declarações das funções e bibliotecas utilizadas em io.c
====================================================================================================================*/

#ifndef IO_H
#define IO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//Processa argumentos da linha de comando do servidor e imprime informações na tela
void processaArgumentosServidor(int argc, char *argv[], int *porto_servidor, int *tam_buffer, int *tam_janela);

//Processa argumentos da linha de comando do cliente e imprime informações na tela
void processaArgumentosCliente(int argc, char *argv[], char **host_do_servidor, int *porto_servidor, char **nome_arquivo, int *tam_buffer, int *tam_janela);

#endif