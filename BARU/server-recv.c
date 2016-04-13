#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

int main(void)
{
    int listenfd = 0;
    int connfd = 0;
    int bytesReceived = 0;
    char recvBuff[256];
    struct sockaddr_in serv_addr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    printf("Socket retrieve success\n");

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(recvBuff, '0', sizeof(recvBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

    if(listen(listenfd, 10) == -1)
    {
        printf("Failed to listen\n");
        return -1;
    }


    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);

        /* Create file where data will be stored */
		FILE *fp;
		fp = fopen("log-w-date.txt", "ab");
		if(NULL == fp)
		{
			printf("Error opening file");
			return 1;
		}

		/* Receive data in chunks of 256 bytes */
		while((bytesReceived = read(connfd, recvBuff, 256)) > 0)
		{
			printf("Bytes received %d\n",bytesReceived);
			// recvBuff[n] = 0;
			fwrite(recvBuff, 1,bytesReceived,fp);
			// printf("%s \n", recvBuff);
		}

		if(bytesReceived < 0)
		{
			printf("\n Read Error \n");
		}

		fclose(fp);

        close(connfd);
        sleep(1);
    }


    return 0;
}
