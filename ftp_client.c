/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 21:48:04 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/27 00:15:01 by maki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "ft_p.h"

int		get_argument(char *cmd)
{
	int		i;

	i = 0;
	while (cmd[i] && !isblank(cmd[i]))
		i++;
	while (cmd[i] && isblank(cmd[i]))
		i++;
	return (i);
}

void	usage(char *bin)
{
	printf("Usage: %s <addr> <port>\n", bin);
	exit(-1);
}

void	error(char *err)
{
	dprintf(2, "%s\n", err);
	exit(-1);
}

int		create_client(char *addr, int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return (-1);
	if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) < 0)
		error("[\e[38;5;1mERROR\e[0m] Error in connection.");
	printf("[\e[38;5;2mSUCCESS\e[0m] Client socket is created.\n");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(addr);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) < 0)
		error("[\e[38;5;1mERROR\e[0m] Error in connection.");
	printf("[\e[38;5;2mSUCCESS\e[0m] Connected to Server.\n");
	return (sock);
}

t_cmd	command(int sock)
{
	t_cmd cmd;

	memset(&cmd, 0, sizeof(cmd));
	if ((cmd.len = read(0, &cmd.str, CMD_MAX - 1)) < 0)
	{
		close(sock);
		exit(EXIT_FAILURE);
	}
	else if (!cmd.len)
	{
		close(sock);
		printf("\e[3mDisconnected from server.\e[0m\n");
		exit(EXIT_SUCCESS);
	}
	else
	{
		cmd.str[cmd.len] = '\0';
		return (cmd);
	}
}

void	send_command(int sock, t_cmd cmd)
{
		printf("%d\n", __LINE__);
	
	if (send(sock, cmd.str, cmd.len, 0) < 0)
	{
		close(sock);
		exit(EXIT_SUCCESS);
	}
	if (strncmp("get ", cmd.str, 4) == 0)
	{
		char bs[1024];
		int size = 0;
		cmd.str[cmd.len - 1] = 0;
		int file = open(cmd.str + get_argument(cmd.str), O_WRONLY | O_CREAT , 0644);
		if (recv(sock, bs, 1024, 0) < 0)
			error("file problem");
		size = atoi(bs);
		char *m = malloc(sizeof(char) * size + 1);
		if (recv(sock, m, size, 0) < 0)
			error("file problem");
		write(file, m, size);
		free(m);
		printf("%d\n", __LINE__);
	}

}

void	recieve_result(int sock)
{
	t_cmd cmd;

		printf("%d\n", __LINE__);

	if ((cmd.len = recv(sock, cmd.str, CMD_MAX, 0)) < 0)
		error("[\e[38;5;1mERROR\e[0m] Error in receiving data.\n");
	else if (cmd.len == 0)
		printf("%d\n", __LINE__);
	else
	{
		printf("%d\n", __LINE__);
		cmd.str[cmd.len] = '\0';
		if (cmd.str[cmd.len - 1] != '\n')
			printf("%s\n", cmd.str);
		else
			printf("%s", cmd.str);
	}
	// while ((cmd.len = recv(sock, cmd.str, CMD_MAX, 0)) > 0)
	// {
	// 	printf("SIZE %d\n", cmd.len);
	// 	cmd.str[cmd.len] = '\0';
	// 	if (cmd.str[cmd.len - 1] != '\n')
	// 		printf("%s\n", cmd.str);
	// 	else
	// 		printf("%s", cmd.str);
	// }
}

int		local_command(int sock, t_cmd cmd)
{
	if (strcmp(cmd.str, "quit\n") == 0)
	{
		send(sock, "quit\n", 5, 0);
		printf("\e[3mDisconnected from server.\e[0m\n");
		close(sock);
		exit(EXIT_SUCCESS);
	}
	return (0);
}

int		main(int ac, char **av)
{
	int					port;
	int					sock;
	t_cmd				cmd;

	if (ac != 3)
		usage(av[0]);
	port = atoi(av[2]);
	if ((sock = create_client(av[1], port)) < 0)
		return (-1);
	while (1)
	{
		printf("%d\n", __LINE__);

		write(1, "ftp> ", 5);
		cmd = command(sock);
		printf("%d\n", __LINE__);

		if (cmd.len > 1 && cmd.str[0] != '\n' && !local_command(sock, cmd))
		{
		printf("%d\n", __LINE__);

			send_command(sock, cmd);
		printf("%d\n", __LINE__);

			// recieve_result(sock);
		printf("%d\n", __LINE__);

			memset(&cmd.str, 0, CMD_MAX);
			printf("%d\n", __LINE__);
		}
		printf("%d\n", __LINE__);
	}
	close(sock);
	return (0);
}
