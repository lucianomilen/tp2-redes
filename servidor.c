#include "common.h"

void closeServer(){
        printf("Reenvios: %d\nPacotes enviados: %d\nPacotes recebidos: %d\nPacotes descartados: %d\nACKs enviados: %d\nACKs recebidos: %d\n",resentPack,sentPack,recPack,dispPack,sentACK,recACK);
        close(sockID);
}


int sendBuff(char *buffer, int buffSize){

        int ACKs = -1, SENT = 0, pack_id;
        char pack[HEADER + buffSize];
        char ACKMSG[HEADER + buffSize];
        char RESENT[HEADER + buffSize];

        createPack(pack, packID, DATA_TYPE, buffer, buffSize);

        ACKs = tp_recvfrom(sockID, ACKMSG, HEADER + buffSize, (so_addr*)&addr);

        //Servidor não recebeu nenhum ACK. É preciso reenviar todos os pacotes do buffer antes de continuar
        while(ACKs == -1) {
                pack_id = 0;
                createPack(RESENT, pack_id, DATA_TYPE, HEADER + recBuff, buffSize);
                tp_sendto(sockID, RESENT, buffSize + HEADER, (so_addr*)&addr);
                sentPack++;
                resentPack++;
                ACKs = tp_recvfrom(sockID, ACKMSG, HEADER + buffSize, (so_addr*)&addr);
        }
        recACK++;

        //Servidor recebe o ACK de um pacote não esperado
        if(getPackID(ACKMSG) != lastACKID) {
                lastACKID = getPackID(ACKMSG);
        }

        lastACKID++;
        strcpy(recBuff, pack);

        //Envia o pacote para o cliente
        SENT = tp_sendto(sockID, pack, HEADER + buffSize, (so_addr*)&addr);
        sentPack++;
        packID++;
        timer(1);

        return SENT;
}

int signalEOF(int buffSize){

        char pack[HEADER + buffSize];

        createPack(pack, packID, FINAL_TYPE, NULL, 0);

        tp_sendto(sockID, pack, HEADER, (so_addr*)&addr);
        sentPack++;

        do {
                tp_recvfrom(sockID, pack, HEADER+buffSize, (so_addr*)&addr);
                recACK++;
        } while(pack[HEADER - 2] != FINAL_TYPE);          //Espera a confirmação do ack final

        timer(1);
        return 1;
}

int startServer(int porto_servidor, int buffSize){

        tp_init();

        sockID = tp_socket(porto_servidor);

        if(sockID < 0) {
                printf("Unable to create server socket\n");
                return -1;
        }

        packID = 0;
        lastACKID = 0;
        recACK = 0;
        sentACK = 0;
        recPack = 0;
        sentPack = 0;
        dispPack = 0;
        resentPack = 0;

        recBuff = (char*) malloc(sizeof(char) * (HEADER + buffSize));

        return sockID;
}

int getFileName(char *buffer, int buffSize){

        char fileNamePack[HEADER + buffSize];         //cria pacote com o nome do arquivo
        int rec_packs = tp_recvfrom(sockID, fileNamePack, HEADER + buffSize, (so_addr*)&addr);
        recPack++;
        strncpy(buffer, HEADER + fileNamePack, buffSize);         //copia para o buffer
        timer(1);
        return rec_packs - HEADER;
}

int main(int argc, char *argv[]){

        //Declaração das variaveis
        int porto_servidor, buffSize, readBytes;
        char *buffer;

        if(argc < 3) {
                printf("Usage: ./servidor <porto_servidor> <buffSize>\n");
                exit(1);
        }

        porto_servidor = atoi(argv[1]);
        buffSize = atoi(argv[2]);

        //Faz abertura passiva e aguarda conexao
        startServer(porto_servidor, buffSize);
        printf("Waiting client connection...\n");

        //Recebe a string com o nome do arquivo
        buffer = (char*) malloc(sizeof(char) * buffSize);
        if(getFileName(buffer, buffSize) < 0) {
                printf("Unable to get file name\n");
                return 0;
        }
        //Abre o arquivo que vai ser lido, se der erro, fecha conexao e termina
        FILE *fp = fopen(buffer, "r");

        if(fp == NULL) {
                printf("Unable to open file\n");
                closeServer();
                return 0;
        }

        //Le o arquivo, um buffer por vez ate fread retornar zero
        readBytes = 0;

        while((readBytes = fread(buffer, 1, buffSize, fp)) != 0) {
                sendBuff(buffer, readBytes);
        }
        //Fecha arquivo e conexao
        if(signalEOF(buffSize) == 1) {
                printf("Arquivo enviado ao cliente!\n");
        }

        free(buffer);
        fclose(fp);
        closeServer();

        printf("\nConnection successfully terminated\n");

        return 0;
}
