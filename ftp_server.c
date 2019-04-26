/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 21:49:33 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/26 19:23:15 by maki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	(void)cmd;
	int file;
	void *ptr;
	struct stat st;
	char tmp[1024];

	if ((file = open(cmd.str + get_argument(cmd.str), O_RDONLY)) < 0)
	{
		send(client_socket, "Error with open.\n", 17, 0);
		return ;
	}
	fstat(file, &st);
	printf("size : %zu\n", st.st_size);
	sprintf(tmp, "%ld", st.st_size);
	send(client_socket, tmp, strlen(tmp), 0);
	printf("%s\n", cmd.str + get_argument(cmd.str));
	if ((ptr = mmap(0, st.st_size, PROT_READ , MAP_PRIVATE, file, 0)) == MAP_FAILED)
		send(client_socket, "mmap error.\n", 12, 0);
	printf("ptr : %p\n", ptr);
	if (ptr) {
		send(client_socket, ptr, st.st_size, 0);
		printf("file send\n");
	}
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
		send(client_socket, "?Invalid command.", 17, 0);
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
				// -> check command and send result
				builtins(client.cs, cmd);
				// if (send(client.cs, "good", 4, 0) < 0)
				// 	break ;
			}
		}
	}
	close(client.cs);
	close(sock);
	return (0);
}
