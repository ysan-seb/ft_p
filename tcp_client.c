#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int	main(int ac, char av)
{
	int network_socket;
	int connection_status;
	char server_response[256];
	struct sockaddr_in server_adress;
	network_socket = socket(AF_INET, SOCK_STREAM, 0);

	server_adress.sin_family = AF_INET;
	server_adress.sin_port = htons(9002);
	server_adress.sin_addr.s_addr = INADDR_ANY;

	connection_status = connect(network_socket, (struct sockaddr *)&server_adress, sizeof(server_adress));
	if (connection_status == -1)
	{
		printf("connection error\n");
		return (-1);
	}
	recv(network_socket, &server_response, sizeof(server_response), 0);
	printf("server: %s\n", server_response);
	close(network_socket);
	return (0);
}
