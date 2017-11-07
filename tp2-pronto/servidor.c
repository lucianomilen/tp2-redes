/*==================================================================================================================
Trabalho Prático 2 - Sockets UDP, janela deslizante                     
Redes de Computadores                               
Aluno: Guilherme Saulo Alves               
servidor.c: Contém o codigo do servidor
====================================================================================================================*/

#include "io.h"
#include "janela.h"

int main(int argc, char *argv[]){
    
    //Declaração das variaveis
    int porto_servidor,tam_buffer,tam_janela,bytes_lidos;
    char *buffer;
    FILE *file;
   
    //Processa argumentos da linha de comando
    processaArgumentosServidor(argc,argv,&porto_servidor,&tam_buffer,&tam_janela);
	  
    //Faz abertura passiva e aguarda conexao
    fazAberturaPassiva(porto_servidor,tam_buffer,tam_janela);
    printf("Aguardando conexão com um cliente..\n");
    	
    //Recebe a string com o nome do arquivo
	buffer=(char*)malloc(sizeof(char)*tam_buffer);   
    if(recebeNomeArquivo(buffer,tam_buffer)<0){
        printf("Erro: Não conseguiu receber o nome do arquivo.\n");
        return 0;
    }
    printf("Nome do arquivo recebido: %s\n",buffer);

    //Abre o arquivo que vai ser lido, se der erro, fecha conexao e termina
    if((file=fopen(buffer,"r"))==NULL){
        printf("Erro: Arquivo %s nao encontrado.\n",buffer);
        fechaConexaoServidor();     
        return 0;
    }

    //Le o arquivo, um buffer por vez ate fread retornar zero
    bytes_lidos=0;
    printf("Enviando o arquivo: %s\n",buffer);
    while((bytes_lidos=fread(buffer,1,tam_buffer,file))!=0) 
        enviaBuffer(buffer,bytes_lidos);
        
    //Fecha arquivo e conexao
    if(enviaFimArquivo(tam_buffer)==1){
    	printf("Arquivo enviado ao cliente!\n");
    }

    free(buffer);
    fclose(file);  
    fechaConexaoServidor();
    printf("\nFim Conexão Servidor!\n");

    return 0;
}
