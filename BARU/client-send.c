#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(void)
{
    int sockfd = 0;
    struct sockaddr_in serv_addr;

    /* Create a socket first */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    /* Initialize sockaddr_in data structure */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); // port
    serv_addr.sin_addr.s_addr = inet_addr("10.1.2.25");
    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    /* Attempt a connection */
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }

	/* Open the file that we wish to transfer */
	FILE *fp = fopen("log-w-date.txt","rb");
	if(fp==NULL)
	{
		printf("File opern error");
		return 1;
	}

	/* Read data from file and send it */
	while(1)
	{
		/* First read file in chunks of 256 bytes */
		unsigned char buff[256]={0};
		int nread = fread(buff,1,256,fp);
		printf("Bytes read %d \n", nread);

		/* If read was success, send data. */
		if(nread > 0)
		{
			printf("Sending \n");
			write(sockfd, buff, nread);
		}

		/*
		 * There is something tricky going on with read ..
		 * Either there was error, or we reached end of file.
		 */
		if (nread < 256)
		{
			if (feof(fp))
				printf("End of file\n");
			if (ferror(fp))
				printf("Error reading\n");
			break;
		}
	}
}
