/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 21:49:33 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/30 02:42:55 by maki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

# define PUT_ABORT "[\e[38;5;1mERROR\e[0m] Command put abort.\n"

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
	char buff[1024];

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
			break;
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
	if (strncmp(cmd.str, "cd", 2) == 0)
		command_cd(client_socket, cmd);
	else if (strncmp(cmd.str, "ls", 2) == 0)
		command_ls(client_socket, cmd);
	else if (strcmp(cmd.str, "pwd") == 0)
		command_pwd(client_socket);
	else if (strncmp(cmd.str, "put", 3) == 0)
		command_put(client_socket, cmd);
	else if (strncmp(cmd.str, "get", 3) == 0)
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
	pid_t				child;
	t_cmd				cmd;
	t_client			c;

	if (ac != 2)
		usage(av[0]);
	port = atoi(av[1]);
	getcwd(cmd.home, PATH_MAX);
	if ((sock = create_server(port)) < 0)
		return (-1);
	while (1)
	{
		if ((c.cs = accept(sock, (struct sockaddr*)&c.csin, &c.cslen)) < 0)
			break ;
		if ((child = fork()) == 0)
		{
			while (1)
			{
				memset(&cmd.str, 0, PATH_MAX);
				if ((cmd.len = recv(c.cs, cmd.str, CMD_MAX - 1, 0)) < 0)
					break ;
				cmd.str[cmd.len] = '\0';
				printf("%s\n", cmd.str);
				builtins(c.cs, cmd);
			}
		}
		// else
		// 	wait(&child);
	}
	close(c.cs);
	close(sock);
	return (0);
}
