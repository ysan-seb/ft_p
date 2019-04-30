/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 21:49:33 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/30 19:36:30 by ysan-seb         ###   ########.fr       */
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
	ft_printf("[\e[38;5;2m+\e[0m]Server socket is created.\n");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) < 0)
		error("[\e[38;5;1m-\e[0m]Error in binding.\n");
	ft_printf("[\e[38;5;2m+\e[0m]Bind to port %d.\n", port);
	if (listen(sock, 42) == 0)
		ft_printf("Listening ...\n");
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
		ft_memset(&buff, 0, 1024);
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
	if (!ft_strcmp(cmd.str, "cd") || !ft_strncmp(cmd.str, "cd ", 3))
		command_cd(client_socket, cmd);
	else if (!ft_strcmp(cmd.str, "ls") || !ft_strncmp(cmd.str, "ls ", 3))
		command_ls(client_socket, cmd);
	else if (!ft_strcmp(cmd.str, "pwd"))
		command_pwd(client_socket);
	else if (!ft_strcmp(cmd.str, "put") || !ft_strncmp(cmd.str, "put ", 4))
		command_put(client_socket, cmd);
	else if (!ft_strcmp(cmd.str, "get") || !ft_strncmp(cmd.str, "get ", 4))
		command_get(client_socket, cmd);
	else if (!ft_strcmp(cmd.str, "quit"))
		command_quit(client_socket);
	else
		send(client_socket, CMD_ERROR, ft_strlen(CMD_ERROR), 0);
	return (0);
}

int		main(int ac, char **av)
{
	int					port;
	int					sock;

	if (ac != 2)
		usage(av[0]);
	port = ft_atoi(av[1]);
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
