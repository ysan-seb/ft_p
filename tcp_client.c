#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 444

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

int good_cmd(char *buffer) {
	int i = 0;

	while (buffer[i]) 
	{
		if (buffer[i++] != ' ')
			return (1);
	}
	return (0);
}

void sig_ignore(int sig) {
	(void)sig;
}

int	main()
{
	int i = 0;
	int ret = 0;
	char c;
	int clientSocket;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	bzero(buffer, 1024);	
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error("[\e[38;5;1m-\e[0m]Error in connection.");
	printf("[\e[38;5;2m+\e[0m]Client socket is created.\n");

	serverAddr = get_sockaddr();
	
	if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
		error("[\e[38;5;1m-\e[0m]Error in connection.");
	printf("[\e[38;5;2m+\e[0m]Connected to Server.\n");
	signal(SIGINT, sig_ignore);
	signal(SIGQUIT, sig_ignore);
	while (1)
	{
		write(1, "ftp> ", 5);
		while ((ret = read(0, &c, 1)) > 0 && i < 1024)
		{
			if (c == '\n')
				break;
			if (c != '\t')
				buffer[i++] = c;
		}
		if (!ret) {
			close(clientSocket);
			printf("\e[3mDisconnected from server.\n");
			exit(1);
		}
		i = 0;
		if (strlen(buffer) > 0 && strcmp(buffer, "clear") != 0 && good_cmd(buffer)) {
			send(clientSocket, buffer, strlen(buffer), 0);
			if (strcmp(buffer, "quit") == 0)
			{
				close(clientSocket);
				printf("\e[3mDisconnected from server.\n");
				exit(1);
			}
			if (recv(clientSocket, buffer, 1024, 0) < 0)
				printf("[\e[38;5;1m-\e[0m]Error in receiving data.\n");
			else
			{
				if (buffer[strlen(buffer) - 1] != '\n')
					printf("%s\n", buffer);
				else
					printf("%s", buffer);
			}
		}
		if (strcmp(buffer, "clear") == 0)
			write(1, "\e[1;1H\e[2J", 12);
		bzero(buffer, 1024);
	}
	return (0);
}
