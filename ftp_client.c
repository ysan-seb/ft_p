/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 21:48:04 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/30 02:35:52 by maki             ###   ########.fr       */
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

// size_t	ftp_send_file_size(int sock, int fd)
// {
// 	struct stat	st;
// 	char		conv[64];

// 	if (fstat(fd, &st) < 0)
// 		error("Error with fstat");
// 	sprintf(conv, "%ld", st.st_size);
// 	if (send(sock, conv, strlen(conv), 0) < 0)
// 		error("Error while sending status.");
// 	return (st.st_size);
// }
 
int	send_command(int sock, t_cmd cmd)
{
	t_file file;
	struct stat st;		
	int			size;
	char		buff[BUFF_SIZE];
	
	if (send(sock, cmd.str, cmd.len, 0) < 0)
	{
		close(sock);
		exit(EXIT_SUCCESS);
	}
	if (strncmp("get", cmd.str, 3) == 0)
		ftp_get_file(sock, cmd);
	if (strncmp("put", cmd.str, 3) == 0)
	{
		cmd.str[cmd.len - 1] = 0;
		if ((file.fd = ftp_open_file(sock, cmd)) < 0)
			return printf("%s", OPEN_ERROR);
		if (fstat(file.fd, &st) < 0)
			error("Error with stat");
		file.size = st.st_size;		
		printf("%zu %d\n", file.size, file.fd);

		while ((size = read(file.fd, buff, BUFF_SIZE - 1)) && size != -1)
		{
			send(sock, buff, size, 0);
			memset(buff, 0, BUFF_SIZE);
		}
		close(file.fd);
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
			while ((cmd.len = read(sock, cmd.str, CMD_MAX)) > 0)
			{
				write(1, cmd.str, cmd.len);
				if (strstr(cmd.str, "SUCCESS") || strstr(cmd.str, "ERROR"))
					break ;
			}
		}
	}
	close(sock);
	return (0);
}
