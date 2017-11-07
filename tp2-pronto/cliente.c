/*==================================================================================================================
Trabalho Prático 2 - Sockets UDP, janela deslizante                     
Redes de Computadores                               
Aluno: Guilherme Saulo Alves               
cliente: Contém o codigo do cliente
====================================================================================================================*/

#include "io.h"
#include "time.h"
#include "janela.h"

int main(int argc, char * argv[]) {
    
    //Declaração das variaveis
    int porto_servidor,tam_buffer,tam_janela;
    char *buffer,*host_do_servidor,*nome_arquivo;
    double tempoInicial,tempoTotal,kbps;
    unsigned int totalBytesRecebidos = 0;
    int bytes_recebidos = 0;
    FILE *file;
  
    //Processa argumentos da linha de comando
    processaArgumentosCliente(argc,argv,&host_do_servidor,&porto_servidor,&nome_arquivo,&tam_buffer,&tam_janela);
     
    //Faz abertura ativa da conexão
    fazAberturaAtiva(host_do_servidor,porto_servidor);
    printf("Conectou com o servidor %s na porta %d.\n\n",host_do_servidor,porto_servidor);

    //Chama gettimeofday para tempo inicial
    tempoInicial=obtemTempo();

    //Envia string com nome do arquivo    
    buffer=(char*)malloc(sizeof(char)*tam_buffer);
    strcpy(buffer,nome_arquivo);
    if(enviaNomeArquivo(buffer,tam_buffer)<0){
		printf("Erro: Não conseguiu enviar o nome do arquivo.\n");
		return 0;
    }
    printf("Enviou o nome do arquivo.\n\n");
 
    //Abre arquivo que vai ser gravado
    strcat(nome_arquivo," (cópia)"); //para nao dar conflito no diretorio
    file=fopen(nome_arquivo,"w");
   
    //Recebe buffer até que perceba que arquivo acabou
    printf("Recebendo arquivo...\n\n"); 
    while((bytes_recebidos=recebeBuffer(buffer,tam_buffer))>0)
        totalBytesRecebidos+=fwrite(buffer,1,bytes_recebidos,file);

    //Fecha arquivo
    printf("Arquivo %s salvo no diretorio\n",nome_arquivo);
    fclose(file);
    free(buffer);
    free(host_do_servidor);
    free(nome_arquivo);

    //Chama gettimeofday para o tempo final e calcula tempo gasto
    tempoTotal=obtemTempo()-tempoInicial;
 
    //Imprime resultado
    kbps=(totalBytesRecebidos/tempoTotal);    
    printf("\nBuffer = \%5u byte(s), \%10.2f kbps (\%u bytes em \%3.06f s)\n",tam_buffer,kbps,totalBytesRecebidos,tempoTotal);

    //Fecha conexão
    fechaConexaoCliente();
    
    printf("\nFim Conexão Cliente!\n");
    
    return 1;
}
