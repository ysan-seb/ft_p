#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "libft/libft.h"

#define PORT 444
#define INV_CMD "?Invalid command.\n"

void	error(char *msg)
{
	dprintf(2, "%s\n", msg);
	exit(1);
}

struct sockaddr_in get_sockaddr()
{
	struct sockaddr_in serverAddr;

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_port = inet_addr("127.0.0.1");
	return (serverAddr);
}

void ls(int newSocket, char **av)
{
    pid_t pidchild;
    extern char **environ;
    
    if ((pidchild = fork()) == 0)
    {
        dup2(newSocket, 1);
        execv("/bin/ls", av);
        exit(1);
    }
    else if (pidchild > 0)
        wait(&pidchild);
}

void av_free(char **av)
{
    int i;

    i = 0;
    while (av[i])
        free(av[i++]);
    free(av);
}

void pwd(int newSocket)
{
    char buffer[1024];

    getcwd(buffer, 1024);
    send(newSocket, buffer, strlen(buffer), 0);
}

int builtin(int newSocket, char *cmd)
{
    int ret;
    char **av;

    ret = 0;
    av = ft_strsplit(cmd, ' ');
    printf("cmd : %s\n", cmd);
    if (strcmp(av[0], "ls") == 0)
    {
        av = ft_strsplit(cmd, ' ');
        ls(newSocket, av);
    }
    else if (strcmp(av[0], "cd") == 0)
        send(newSocket, "cd", 2, 0);                
    else if (strcmp(av[0], "get") == 0)
        send(newSocket, "get", 3, 0);            
    else if (strcmp(av[0], "put") == 0)
        send(newSocket, "put", 3, 0);
    else if (strcmp(av[0], "pwd") == 0)
        pwd(newSocket);
    else if (strcmp(av[0], "quit") == 0)
    {
        ret = -1;
        send(newSocket, "quit", 4, 0);
    }
    else
        send(newSocket, INV_CMD, strlen(INV_CMD), 0);
    av_free(av);
    return (ret);
}

int	main()
{
    int sockfd;
    int newSocket;
	struct sockaddr_in serverAddr;
    struct sockaddr_in newAddr;
    socklen_t addr_size;
    char buffer[1024];
    pid_t childpid;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error("[\e[38;5;1m-\e[0m]Error in connection.\n");
	printf("[\e[38;5;2m+\e[0m]Server socket is created.\n");

	serverAddr = get_sockaddr();

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
		error("[\e[38;5;1m-\e[0m]Error in binding.\n");
    printf("[\e[38;5;2m+\e[0m]Bind to port.\n");

    if (listen(sockfd, 10) == 0)
        printf("Listening....\n");
    else
        printf("[\e[38;5;1m-\e[0m]Error in binding.\n");
    
    while (1)
    {
        if ((newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size)) < 0)
            exit(1);
        printf("Connection accepted from %s\n", inet_ntoa(newAddr.sin_addr));
        if ((childpid = fork()) == 0)
        {
            close(sockfd);
            while (1)
            {
                recv(newSocket, buffer, 1024, 0);
                if (builtin(newSocket, buffer) < 0)
                {
                    printf("Disconnected from %s\n", inet_ntoa(newAddr.sin_addr));
                    break;
                }
                bzero(buffer, 1024);
            }
        }
    }
    close(sockfd);
    return (0);
}