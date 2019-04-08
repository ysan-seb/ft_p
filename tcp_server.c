#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 444

int	main()
{
    int sockfd;
    int ret;
    int newSocket;
	struct sockaddr_in serverAddr;
    struct sockaddr_in newAddr;
    socklen_t addr_size;
    char buffer[1024];
    pid_t childpid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("[\e[38;5;1m-\e[0m]Error in connection.\n");
		exit(1);
	}
	printf("[\e[38;5;2m+\e[0m]Server socket is created.\n");
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_port = inet_addr("127.0.0.1");
    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    	if (ret < 0)
	{
		printf("[\e[38;5;1m-\e[0m]Error in binding.\n");
		exit(1);
	}
    printf("[\e[38;5;2m+\e[0m]Bind to port.\n");
    if (listen(sockfd, 10) == 0)
        printf("Listening....\n");
    else
        printf("[\e[38;5;1m-\e[0m]Error in binding.\n");
    while (1)
    {
        newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
        if (newSocket < 0)
            exit(1);
        printf("Connection accepted from %s\n", inet_ntoa(newAddr.sin_addr));
        if ((childpid = fork()) == 0)
        {
            close(sockfd);
            while (1)
            {
                recv(newSocket, buffer, 1024, 0);
                if (strcmp(buffer, "exit") == 0)
                {
                    printf("Disconnected from %s\n", inet_ntoa(newAddr.sin_addr));
                    break;
                }
                else
                {
                    printf("Client: %s\n", buffer);
                    send(newSocket, buffer, strlen(buffer), 0);
                    bzero(buffer, sizeof(buffer));
                }
            }
        }
    }
    close(sockfd);
    return (0);
}