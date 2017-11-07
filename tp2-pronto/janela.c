/*==================================================================================================================
Trabalho Prático 2 - Sockets UDP, janela deslizante                     
Redes de Computadores                               
Aluno: Guilherme Saulo Alves                         
janela.c: Protocolo UDP usando janela deslizante
====================================================================================================================*/

#include "janela.h"

void criaJanela(int porto_servidor, int tam_buffer, int tam_janela){

	int i;
    id_pacote_=0;
    idUltimoACK_=0;
    tamjanela_=tam_janela;
    tam_janela_atual_=0;
    deslocamento_=0;
    acksrecebidos_=0;
    acksenviados_=0;
    pacotesrecebidos_=0;
    pacotesenviados_=0;
    pacotesdescartados_=0;
    pacotesreenviados_=0;

    janela_=(char**)malloc(sizeof(char*)*tam_janela);
    for(i=0;i<tam_janela;i++){   
        janela_[i] = (char*)malloc(sizeof(char)*(TAM_CABECALHO+tam_buffer));
    }
}

char checksum(char *buffer, int tam_buffer){

    int i;
    int sum=0;
    for(i=0;i<tam_buffer;i++){
        sum+=buffer[i];
        sum=sum%128;
    }
    return sum;
}

void criaPacote(char *pacote, int id_pacote, int tipo_pacote, char *buffer, int tam_buffer){

	/* 	   	  Formato do pacote:
	| ID_DO_PACOTE | TIPO_DO_PACOTE | CHECKSUM | DADOS      | 
	| 4Bytes       | 1Byte          | 1 Byte   | tam_buffer |*/

    memcpy(pacote,&id_pacote,4); //Copia 4 bytes que representa o id do pacote para o inicio do bloco do pacote
    pacote[4]=tipo_pacote; //Copia para a posição 4 o tipo da mensagem
    pacote[5]=checksum(buffer,tam_buffer); //copia para a posição 5 o valor checksum (inteiro de 1 byte)
    strncpy(TAM_CABECALHO+pacote,buffer,tam_buffer); //copia os dados para as posições seguintes
} 

int fazAberturaPassiva(int porto_servidor, int tam_buffer, int tam_janela){

    tp_init();
    
    if((idsocket_=tp_socket(porto_servidor))<0){
        printf("Erro ao criar socket servidor\n");
        return -1;
    }

    criaJanela(porto_servidor,tam_buffer,tam_janela);

    return idsocket_;
}

int fazAberturaAtiva(char *host,int porto){

    tp_init();
    
    if((idsocket_=tp_socket(0))<0){
        printf("Erro ao criar socket cliente\n");
        return -1;
    }
	
    if(tp_build_addr(&endereco_,host,porto)<0){
        printf("Erro ao vincular (host,porto) do servidor\n");
        return -1;
    }
	
    return idsocket_;
}

int recebeNomeArquivo(char *buffer, int tam_buffer){

    char pacote_nome_arquivo[TAM_CABECALHO+tam_buffer]; //cria pacote com o nome do arquivo   
    int pacotes_recebidos=0;       
    pacotes_recebidos=tp_recvfrom(idsocket_,pacote_nome_arquivo,TAM_CABECALHO+tam_buffer,(so_addr*)&endereco_);
    pacotesrecebidos_++;
    strncpy(buffer,TAM_CABECALHO+pacote_nome_arquivo,tam_buffer); //copia para o buffer
	temporizador(1);  
    return pacotes_recebidos-TAM_CABECALHO;
}

int enviaNomeArquivo(char *buffer, int tam_buffer){

    char pacote[tam_buffer+TAM_CABECALHO];
    criaPacote(pacote,id_pacote_,TIPO_DADOS,buffer,tam_buffer);

    int enviados = 0;
    enviados = tp_sendto(idsocket_, pacote, tam_buffer+TAM_CABECALHO, (so_addr*)&endereco_);
    pacotesenviados_++;

    return enviados; 
}

int idPacote(char *pacote){

	int id_pacote;
    memcpy(&id_pacote,&pacote[0],4);

    return id_pacote;
}

int enviaBuffer(char *buffer, int tam_buffer){

	int recebeuack=-1, pctsenviados=0, id_pacote=0;
    char pacote[TAM_CABECALHO+tam_buffer];
   	char pacoteack[TAM_CABECALHO+tam_buffer];
   	char pacotereenvio[TAM_CABECALHO+tam_buffer];
   
   	criaPacote(pacote,id_pacote_,TIPO_DADOS,buffer,tam_buffer);
	
	//Caso a Janela esteja CHEIA  
   	if(tam_janela_atual_==tamjanela_){
		
		recebeuack=tp_recvfrom(idsocket_,pacoteack,TAM_CABECALHO+tam_buffer,(so_addr*)&endereco_);

        //Servidor não recebeu nenhum ACK. É preciso reenviar todos os pacotes do buffer antes de mover a janela deslizante
        while(recebeuack==-1){  
        	int i;
          	for(i=deslocamento_; i<tamjanela_; i++){
          		id_pacote=idPacote(janela_[i%tamjanela_]);
          		criaPacote(pacotereenvio,id_pacote,TIPO_DADOS,TAM_CABECALHO+janela_[i],tam_buffer);
            	tp_sendto(idsocket_, pacotereenvio, tam_buffer+TAM_CABECALHO,(so_addr*)&endereco_);
            	pacotesenviados_++;
            	pacotesreenviados_++;
          	}
        	recebeuack=tp_recvfrom(idsocket_,pacoteack,TAM_CABECALHO+tam_buffer,(so_addr*)&endereco_);
        }
        acksrecebidos_++;

        //Servidor recebe o ACK de um pacote não esperado
        if(idPacote(pacoteack)!=idUltimoACK_){
        	tam_janela_atual_-=idPacote(pacoteack)-idUltimoACK_-1;
          	idUltimoACK_=idPacote(pacoteack);
        }

        //Desliza Janela e copia pacote para a janela
       	idUltimoACK_++;    
       	strcpy(janela_[deslocamento_],pacote);
       	deslocamento_=(deslocamento_+1)%tamjanela_;
	}
    
    //Caso a janela ainda tenha ESPAÇO LIVRE
    else{
        tam_janela_atual_++;
        strcpy(janela_[(tam_janela_atual_-1+deslocamento_)%tamjanela_],pacote); //Copia o pacote para a primeira posição vazia da janela
    }

    //Envia o pacote para o cliente
    pctsenviados=tp_sendto(idsocket_,pacote,TAM_CABECALHO+tam_buffer,(so_addr*)&endereco_);
    pacotesenviados_++;
    id_pacote_++;
 	temporizador(1);

    return pctsenviados;
}

int recebeBuffer(char *buffer, int tam_buffer){

    int bytesRecebidos=0;
    char pacote[TAM_CABECALHO+tam_buffer];
    char pacoteack[TAM_CABECALHO+tam_buffer];
    
    bytesRecebidos=tp_recvfrom(idsocket_,pacote,TAM_CABECALHO+tam_buffer,(so_addr*)&endereco_);
    pacotesrecebidos_++;

    //Cliente recebeu um pacote que já foi recebido (servidor não recebeu o ACK daquele pacote)
    while(idPacote(pacote)!=id_pacote_){
    	criaPacote(pacoteack,idPacote(pacote),TIPO_ACK,TAM_CABECALHO+pacote,tam_buffer); 
        tp_sendto(idsocket_,pacote,TAM_CABECALHO+tam_buffer,(so_addr*)&endereco_);
        acksenviados_++;
        //o pacote é descartado
        bytesRecebidos=tp_recvfrom(idsocket_,pacote,TAM_CABECALHO+tam_buffer,(so_addr*)&endereco_);
        pacotesdescartados_++;
    }

    //Caso o pacote recebido seja do TIPO_FINAL
    if(pacote[TAM_CABECALHO-2]==TIPO_FINAL){
        criaPacote(pacoteack,idPacote(pacote),TIPO_FINAL, TAM_CABECALHO+pacote,tam_buffer); 
		tp_sendto(idsocket_,pacoteack,TAM_CABECALHO+tam_buffer,(so_addr*)&endereco_);
    }
    
    //Cliente recebe pacote esperado e envia um pacote ACK para o servidor
    else{
	/*	char verificaChecksum=checksum(pacote,sizeof(pacote));
    	if(pacote[TAM_CABECALHO-1]!=verificaChecksum){
			//Aqui deveria ser o tratamento de erro checksum
  		}
	*/	
		criaPacote(pacoteack,idPacote(pacote),TIPO_ACK,TAM_CABECALHO+pacote,tam_buffer); 
		tp_sendto(idsocket_,pacoteack,TAM_CABECALHO+tam_buffer,(so_addr*)&endereco_);
	}
	
   	acksenviados_++;
    id_pacote_++;

    //Copia a mensagem para o buffer
	strncpy(buffer,TAM_CABECALHO+pacote,tam_buffer);

    return bytesRecebidos-TAM_CABECALHO;
}

int enviaFimArquivo(int tam_buffer){

    char pacote[TAM_CABECALHO+tam_buffer];
    
    criaPacote(pacote,id_pacote_,TIPO_FINAL,NULL,0);
    
    tp_sendto(idsocket_,pacote,TAM_CABECALHO,(so_addr*)&endereco_);
    pacotesenviados_++;

    do{
    	tp_recvfrom(idsocket_,pacote,TAM_CABECALHO+tam_buffer,(so_addr*)&endereco_);
    	acksrecebidos_++;
    }while(pacote[TAM_CABECALHO-2]!=TIPO_FINAL); //Espera a confirmação do ack final
  
    temporizador(1);
    return 1;
}

void fechaConexaoServidor(){

    printf("\n## INFORMACOES ##\n");
    printf("Reenvios: %d\nPacotes enviados: %d\nPacotes recebidos: %d\nPacotes descartados: %d\nACKs enviados: %d\nACKs recebidos: %d\n",pacotesreenviados_,pacotesenviados_,pacotesrecebidos_,pacotesdescartados_,acksenviados_,acksrecebidos_);
    close(idsocket_);
}

void fechaConexaoCliente(){
    
    printf("\n## INFORMACOES ##\n");
    printf("Reenvios: %d\nPacotes enviados: %d\nPacotes recebidos: %d\nACKs enviados: %d\nACKs recebidos: %d\n",pacotesreenviados_,pacotesenviados_,pacotesrecebidos_,acksenviados_,acksrecebidos_);
    close(idsocket_);
}
