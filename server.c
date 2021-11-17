#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <uk/assert.h>
#include <unistd.h>
#include <errno.h>

int run_server(int RECVBUFFERSIZE)
{
    int sockfd, connfd, rc;
    int len;
    struct sockaddr_in cli;
    struct sockaddr_in servaddr;
    char *buf = malloc(1024 * 512 * sizeof(char));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket failed\n");
        return 0;
    }

    for (size_t i = 0; i < sizeof(servaddr); i++)
        *((char *) &servaddr) = 0;

    servaddr.sin_family = AF_INET;
    /* no need for gates here, these are simple calculation macros */
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(12345);

    rc = bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if (rc != 0) {
        printf("Bind failed\n");
        return 0;
    }

    rc = listen(sockfd, 5);
    if (rc != 0) {
        printf("Listen failed\n");
        return 0;
    }

    len = sizeof(cli);

    connfd = accept(sockfd, (struct sockaddr *) &cli, &len);

    if (connfd < 0) {
        printf("Accept failed\n");
        return 0;
    }

    while (1) {
	rc = recv(connfd, buf, RECVBUFFERSIZE, 0);

        if (rc < 0) {
            printf("Read failed with %d\n", rc);
            break;
        }
    }

    return 0;
}
