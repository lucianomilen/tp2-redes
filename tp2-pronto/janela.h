/*==================================================================================================================
Trabalho Prático 2 - Sockets UDP, janela deslizante                     
Redes de Computadores                               
Aluno: Guilherme Saulo Alves                         
janela.h: Contém as declarações das funções e bibliotecas utilizadas em janela.c
====================================================================================================================*/

#ifndef JANELA_H
#define JANELA_H

#include "time.h"
#include "tp_socket.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define TAM_CABECALHO 6
#define TIPO_DADOS 0
#define TIPO_ACK 1
#define TIPO_FINAL 2

//identificadores do socket
struct sockaddr_in endereco_;
int idsocket_;
int id_pacote_;
int idUltimoACK_;

//variaveis da janela
char **janela_;
int deslocamento_;
int tam_janela_atual_;
int tamjanela_;

//estatisticas
int pacotesenviados_;
int pacotesrecebidos_;
int pacotesdescartados_;
int pacotesreenviados_;
int acksrecebidos_;
int acksenviados_;

//Cria uma janela e inicializar as variaveis que gerencia a mesma
void criaJanela(int porto_servidor,int tam_buffer,int tam_janela);

//Retorna o checksum de um pacote
char checksum(char *buffer, int tam_buffer);

//Cria uma pacote respeitando o seu formato
void criaPacote(char *pacote, int id_pacote, int tipo_pacote, char *buffer, int tam_buffer);

//Faz a abertura passiva da conexao e retorna o id do socket criado
int fazAberturaPassiva(int porto_servidor,int tam_buffer,int tam_janela);

//Faz a abertura ativa da conexao e retorna o id do socket criado
int fazAberturaAtiva(char *host,int porto);

//Recebe o nome do arquivo e copia para o buffer. Retorna um inteiro maior que 0 caso não apresente erro
int recebeNomeArquivo(char *buffer, int tam_buffer);

//Envia o nome do arquivo para o servidor. Retorna um inteiro maior que 0 caso não apresente erro
int enviaNomeArquivo(char *buffer, int tam_buffer);

//Retorna o id do pacote passado como parametro
int idPacote(char *pacote);

//Envia o buffer lido do arquivo e retorna o numero de bytes enviados
int enviaBuffer(char *buffer, int tam_buffer);

//Recebe o buffer enviado do servidor e retorna o numero de bytes recebidos
int recebeBuffer(char *buffer, int tam_buffer);

//Envia um pacote do tipo FINAL e retorna 1 caso ele seje confirmado
int enviaFimArquivo(int tam_buffer);

//Imprime algumas informações na tela e fecha a conexao
void fechaConexaoServidor();

////Imprime algumas informações na tela e fecha a conexao
void fechaConexaoCliente();

#endif