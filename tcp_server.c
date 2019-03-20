#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

int     main(int ac, char av)
{
    int server_socket;
    int client_socket;
    char server_message[256] = "You have reached the server!";
    struct sockaddr_in server_adress;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_adress.sin_family = AF_INET;
	server_adress.sin_port = htons(9002);
	server_adress.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*) &server_adress, sizeof(server_adress));
    listen(server_socket, 5);
    client_socket = accept(server_socket, NULL, NULL);
    send(client_socket, server_message, sizeof(server_message), 0);
    close(server_socket);
    return (0);
}
