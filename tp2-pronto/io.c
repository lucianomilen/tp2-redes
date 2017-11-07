/*==================================================================================================================
Trabalho Prático 2 - Sockets UDP, janela deslizante                     
Redes de Computadores                               
Aluno: Guilherme Saulo Alves                         
io.c: Realiza a comunicação do programa com o ambiente
====================================================================================================================*/

#include "io.h"

void processaArgumentosServidor(int argc, char *argv[], int *porto_servidor, int *tam_buffer, int *tam_janela){

    if(argc<4){
        printf("Erro: Digite os parâmetros corretos!\n");
        printf("./servidor <porto_servidor> <tam_buffer> <tam_janela>\n");
        exit(1);
    }

    else{
	    *porto_servidor=atoi(argv[1]);
	    *tam_buffer=atoi(argv[2]);
	    *tam_janela=atoi(argv[3]);
	
		//Limpa tela do terminal e imprime algumas informaçoes
    	printf("\e[2J\e[H"); 
    	printf("## Servidor ##\n");
    	printf("porto_servidor: %d tam_buffer: %d tam_janela: %d\n\n", *porto_servidor,*tam_buffer,*tam_janela);
    }
}

void processaArgumentosCliente(int argc, char *argv[], char **host_do_servidor, int *porto_servidor, char **nome_arquivo, int *tam_buffer, int *tam_janela){

   if(argc<6){
    	printf("Erro: Digite os parâmetros corretos!\n");
        printf("./cliente <host_do_servidor> <porto_servidor> <nome_arquivo> <tam_buffer> <tam_janela>\n");
        exit(1);
    }    

    else{
    	*host_do_servidor=(char*)malloc(sizeof(char)*strlen(argv[1]));
    	strcpy(*host_do_servidor, argv[1]);
   		*porto_servidor=atoi(argv[2]);
   		*nome_arquivo=(char*)malloc(sizeof(char)*(strlen(argv[3])+8));
   		strcpy(*nome_arquivo, argv[3]);
    	*tam_buffer=atoi(argv[4]);
    	*tam_janela=atoi(argv[5]);
    
    	//Limpa tela do terminal e imprime algumas informaçoes
    	printf("\e[2J\e[H"); 
    	printf("## Cliente ##\n");
    	printf("host_do_servidor: %s porto_servidor: %d nome_arquivo: %s tam_buffer: %d tam_janela: %d\n\n", *host_do_servidor,*porto_servidor,*nome_arquivo,*tam_buffer,*tam_janela);
    }
}