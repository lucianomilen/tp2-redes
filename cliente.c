#include "common.h"

int recebeBuffer(char *buffer, int buffSize){

        int recBytes = 0;
        char pack[HEADER + buffSize];
        char ACKMSG[HEADER + buffSize];

        recBytes=tp_recvfrom(sockID, pack, HEADER + buffSize, (so_addr*)&addr);
        recPack++;

        //Cliente recebeu um pacote que já foi recebido (servidor não recebeu o ACK daquele pacote)
        while(getPackID(pack) != packID) {
                createPack(ACKMSG, getPackID(pack), ACK_TYPE, HEADER + pack, buffSize);
                tp_sendto(sockID, pack, HEADER + buffSize, (so_addr*)&addr);
                sentACK++;
                //o pacote é descartado
                recBytes = tp_recvfrom(sockID, pack, HEADER + buffSize, (so_addr*)&addr);
                dispPack++;
        }

        //Caso o pacote recebido seja do FINAL_TYPE
        if(pack[HEADER - 2] == FINAL_TYPE) {
                createPack(ACKMSG, getPackID(pack), FINAL_TYPE, HEADER + pack, buffSize);
                tp_sendto(sockID, ACKMSG, HEADER + buffSize, (so_addr*)&addr);
        }

        //Cliente recebe pacote esperado e envia um pacote ACK para o servidor
        else{
                createPack(ACKMSG, getPackID(pack), ACK_TYPE, HEADER + pack, buffSize);
                tp_sendto(sockID, ACKMSG, HEADER + buffSize, (so_addr*)&addr);
        }

        sentACK++;
        packID++;

        //Copia a mensagem para o buffer
        strncpy(buffer, HEADER + pack, buffSize);
        return recBytes - HEADER;
}

int sendFileName(char *buffer, int buffSize){

        char pack[buffSize + HEADER];
        createPack(pack, packID, DATA_TYPE, buffer, buffSize);

        int sent = 0;
        sent = tp_sendto(sockID, pack, buffSize + HEADER, (so_addr*)&addr);
        sentPack++;

        return sent;
}

int startClient(char *host, int porto){

        tp_init();
        sockID = tp_socket(0);

        if(sockID < 0) {
                printf("Client socket failed\n");
                return -1;
        }

        if(tp_build_addr(&addr, host, porto) < 0) {
                printf("Client server bind failed\n");
                return -1;
        }

        return sockID;
}

int main(int argc, char * argv[]) {

        int porto_servidor,buffSize;
        char *buffer,*serverHost,*fileName;
        double initTime,elapsedTime, endTime,kbps;
        unsigned int sumRecBytes = 0;
        int recBytes = 0;
        FILE *fp;

        //Processa argumentos da linha de comando
        if(argc < 5) {
                printf("Usage: ./cliente <serverHost> <porto_servidor> <fileName> <buffSize>\n");
                exit(1);
        }

        serverHost = (char*) malloc(sizeof(char) * strlen(argv[1]));
        strcpy(serverHost, argv[1]);
        porto_servidor = atoi(argv[2]);
        fileName = (char*) malloc(sizeof(char) * (strlen(argv[3]) + 8));
        strcpy(fileName, argv[3]);
        buffSize = atoi(argv[4]);

        //Faz abertura ativa da conexão
        startClient(serverHost, porto_servidor);

        //Chama gettimeofday para tempo inicial

        struct timeval time;
        gettimeofday(&time, NULL);

        initTime = time.tv_sec + (time.tv_usec/1000000.0);

        //Envia string com nome do arquivo
        buffer = (char*) malloc(sizeof(char) * buffSize);
        strcpy(buffer, fileName);

        if(sendFileName(buffer, buffSize) < 0) {
                printf("Unable to send file name\n");
                return 0;
        }

        //Abre arquivo que vai ser gravado
        strcat(fileName, "_"); //para nao dar conflito no diretorio
        fp = fopen(fileName, "w");

        //Recebe buffer até que perceba que arquivo acabou
        while((recBytes = recebeBuffer(buffer, buffSize)) > 0)
                sumRecBytes += fwrite(buffer, 1, recBytes, fp);

        //Fecha arquivo
        fclose(fp);
        free(buffer);
        free(serverHost);
        free(fileName);

        //Chama gettimeofday para o tempo final e calcula tempo gasto
        struct timeval end;
        gettimeofday(&end, NULL);
        endTime = end.tv_sec + (end.tv_usec/1000000.0);
        elapsedTime = endTime - initTime;

        //Imprime resultado
        kbps = (sumRecBytes / elapsedTime);
        printf("\nBuffer = \%5u byte(s), \%10.2f kbps (\%u bytes em \%3.06f s)\n",buffSize,kbps,sumRecBytes,elapsedTime);

        //Fecha conexão
        printf("Reenvios: %d\nPacotes enviados: %d\nPacotes recebidos: %d\nACKs enviados: %d\nACKs recebidos: %d\n",resentPack,sentPack,recPack,sentACK,recACK);
        close(sockID);

        printf("\nFim Conexão Cliente!\n");

        return 1;
}
