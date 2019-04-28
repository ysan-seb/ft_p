/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 21:48:04 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/29 00:31:03 by maki             ###   ########.fr       */
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
	int fd;
	int ret;
	t_file file;

	ret = 0;
	memset(&file, 0, sizeof(file));
	if (send(sock, cmd.str, cmd.len, 0) < 0)
	{
		close(sock);
		exit(EXIT_SUCCESS);
	}
	if (strncmp("get ", cmd.str, 4) == 0)
	{
		cmd.str[cmd.len - 1] = 0;
		if ((ret = recv(sock, file.open , 1, 0)) < 0)
			error("file problem");
		if (file.open[0] == '0')
			return ;
		else
		{
			send(sock, "OK", 2, 0);
			if ((ret = recv(sock, file.conv, CNV_MAX, 0)) < 0)
				error("file problem");
			send(sock, "OK", 2, 0);
			file.conv[ret] = '\0';
			if ((fd = open(cmd.str + get_argument(cmd.str), O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
				error("fucking open");
			file.size = atoi(file.conv);
				printf("Good %zu\n", file.size);
			if (file.size > 0)
			{
				file.content = malloc(sizeof(char) * (file.size + 1));
				if (recv(sock, file.content, file.size, 0) < 0)
					error("file problem");
				write(fd, file.content, file.size);
				free(file.content);
			}
				send(sock, "OK", 2, 0);				
			close(fd);
		}
	}
}

void	recieve_result(int sock)
{
	t_cmd cmd;

	if ((cmd.len = recv(sock, cmd.str, CMD_MAX, 0)) < 0)
		error("[\e[38;5;1mERROR\e[0m] Error in receiving data.\n");
	else if (cmd.len == 0)
		printf("%d\n", __LINE__);
	else
	{
		cmd.str[cmd.len] = '\0';
		if (cmd.str[cmd.len - 1] != '\n')
			printf("[%s]\n", cmd.str);
		else
			printf("[%s]", cmd.str);
	}
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
		write(1, "ftp> ", 5);
		cmd = command(sock);
		if (cmd.len > 1 && cmd.str[0] != '\n' && !local_command(sock, cmd))
		{
			send_command(sock, cmd);
			memset(&cmd, 0, sizeof(cmd));
			// recieve_result(sock);
			while ((cmd.len = read(sock, cmd.str, CMD_MAX)) > 0)
			{
				write(1, cmd.str, cmd.len);
				if (strstr(cmd.str, "SUCCESS") || strstr(cmd.str, "ERROR"))
					break;
			}
			// len -1
		}
	}
	close(sock);
	return (0);
}
