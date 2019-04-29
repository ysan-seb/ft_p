/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 21:49:33 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/29 17:51:37 by ysan-seb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

#define CMD_ERROR "[\e[38;5;1mERROR\e[0m] Invalid command.\n"
#define GET_OPEN_ERROR "[\e[38;5;1mERROR\e[0m] Error with open.\n"
#define GET_MMAP_ERROR "[\e[38;5;1mERROR\e[0m] Error with mmap.\n"
#define GET_SUCCESS "[\e[38;5;2mSUCCESS\e[0m] File has been send.\n"
#define FALSE "0"
#define TRUE "1"
#define BUFF_SIZE 1024

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

void	command_put(int client_socket, t_cmd cmd)
{
	(void)cmd;
	send(client_socket, "put", 3, 0);
}

void	command_get(int client_socket, t_cmd cmd)
{
	ftp_send_file(client_socket, cmd);
}

void	command_quit(int client_socket)
{
	send(client_socket, "quit", 4, 0);
	close(client_socket);
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
	else if (strcmp(cmd.str, "put") == 0)
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
	pid_t				child;
	t_cmd				cmd;
	t_client			client;

	if (ac != 2)
		usage(av[0]);
	port = atoi(av[1]);
	getcwd(cmd.home, PATH_MAX);
	if ((sock = create_server(port)) < 0)
		return (-1);
	while (1)
	{
		if ((client.cs = accept(sock, (struct sockaddr*)&client.csin, &client.cslen)) < 0)
			break ;
		if ((child = fork()) == 0)
		{
			while (1)
			{
				if ((cmd.len = recv(client.cs, cmd.str, CMD_MAX - 1, 0)) < 0)
					break ;
				cmd.str[cmd.len] = '\0';
				printf("%s", cmd.str);
				builtins(client.cs, cmd);
			}
		}
		// else
		// 	wait(&child);
	}
	close(client.cs);
	close(sock);
	return (0);
}
