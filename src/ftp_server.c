/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 21:49:33 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/30 18:26:28 by ysan-seb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

int		create_server(int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return (-1);
	if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) < 0)
		error("[\e[38;5;1m-\e[0m]Error in connection.\n");
	printf("[\e[38;5;2m+\e[0m]Server socket is created.\n");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) < 0)
		error("[\e[38;5;1m-\e[0m]Error in binding.\n");
	printf("[\e[38;5;2m+\e[0m]Bind to port %d.\n", port);
	if (listen(sock, 42) == 0)
		printf("Listening ...\n");
	else
		error("[\e[38;5;1m-\e[0m]Error in binding.\n");
	return (sock);
}

int		command_put(int sock, t_cmd cmd)
{
	t_file	file;
	int		size;
	char	buff[1024];

	file.name = filename(cmd.str + arg(cmd.str));
	if ((file.fd = open(file.name, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
		error("Error with open.\n");
	while ((size = recv(sock, buff, 1023, 0)) > 0 && size != -1)
	{
		write(file.fd, buff, size);
		memset(&buff, 0, 1024);
		if (size == 1023)
			continue;
		else
			break ;
	}
	close(file.fd);
	ftp_request_status(sock, PUT_SUCCESS, 0);
	return (0);
}

void	command_get(int client_socket, t_cmd cmd)
{
	ftp_send_file(client_socket, cmd);
}

int		builtins(int client_socket, t_cmd cmd)
{
	if (cmd.str[cmd.len - 1] == '\n')
		cmd.str[cmd.len - 1] = '\0';
	if (strcmp(cmd.str, "cd") == 0 || strncmp(cmd.str, "cd ", 3) == 0)
		command_cd(client_socket, cmd);
	else if (strcmp(cmd.str, "ls") == 0 || strncmp(cmd.str, "ls ", 3) == 0)
		command_ls(client_socket, cmd);
	else if (strcmp(cmd.str, "pwd") == 0)
		command_pwd(client_socket);
	else if (strcmp(cmd.str, "put") == 0 || strncmp(cmd.str, "put ", 4) == 0)
		command_put(client_socket, cmd);
	else if (strcmp(cmd.str, "get") == 0 || strncmp(cmd.str, "get ", 4) == 0)
		command_get(client_socket, cmd);
	else if (strcmp(cmd.str, "quit") == 0)
		command_quit(client_socket);
	else
		send(client_socket, CMD_ERROR, strlen(CMD_ERROR), 0);
	return (0);
}

int		main(int ac, char **av)
{
	int					port;
	int					sock;

	if (ac != 2)
		usage(av[0]);
	port = atoi(av[1]);
	if ((sock = create_server(port)) < 0)
		return (-1);
	signal(SIGCHLD, ftp_signal);
	while (1)
	{
		if (!ftp_fork(sock))
			break ;
	}
	close(sock);
	return (0);
}
