#include "common.h"

int getPackID(char *pack){

								int pack_id;
								memcpy(&pack_id,&pack[0],4);

								return pack_id;
}

char checksum(char *buffer, int buffSize){

								int i;
								int sum=0;
								for(i=0; i<buffSize; i++) {
																sum+=buffer[i];
																sum=sum%128;
								}
								return sum;
}

void timer(unsigned int sec){

								struct timeval tv;
								tv.tv_sec=sec;
								tv.tv_usec=0;

								setsockopt(sockID,SOL_SOCKET,SO_RCVTIMEO,(struct timeval*)&tv, sizeof(struct timeval));
}

void createPack(char *pack, int packID, int packType, char *buffer, int buffSize){

								memcpy(pack, &packID, 4);         //Copia 4 bytes que representa o id do pacote para o inicio do bloco do pacote
								pack[4] = packType;         //Copia para a posição 4 o tipo da mensagem
								pack[5] = checksum(buffer,buffSize);         //copia para a posição 5 o valor checksum (inteiro de 1 byte)
								strncpy(HEADER+pack,buffer,buffSize);         //copia os dados para as posições seguintes
}
