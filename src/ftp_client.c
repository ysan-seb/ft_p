/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 21:48:04 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/30 18:14:20 by ysan-seb         ###   ########.fr       */
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

int		ftp_output(char *msg, int ret)
{
	printf("%s", msg);
	return (ret);
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
	if (strcmp(addr, "localhost") == 0)
		sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	else
		sin.sin_addr.s_addr = inet_addr(addr);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) < 0)
		error("[\e[38;5;1mERROR\e[0m] Error in connection.");
	printf("[\e[38;5;2mSUCCESS\e[0m] Connected to Server.\n");
	return (sock);
}

int		ftp_get(int sock, t_cmd cmd)
{
	int			fd;
	struct stat	st;
	void		*ptr;

	if (strcmp(cmd.str, "put") == 0 || strlen(cmd.str + arg(cmd.str)) == 0)
		return (ftp_output(MISSING_ARG, -1));
	if ((fd = open(cmd.str + arg(cmd.str), O_RDONLY)) < 0)
		return (ftp_output(OPEN_ERROR, -1));
	if (fstat(fd, &st) < 0)
		return (ftp_output(STAT_ERROR, -1));
	if ((st.st_mode & S_IFMT) == S_IFDIR)
		return (ftp_output(IS_DIR, -1));
	if (send(sock, cmd.str, cmd.len, 0) < 0)
		error("Error with send.\n");
	if (st.st_size > 0)
	{
		if ((ptr = mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MF)
			error("Error with mmap.\n");
		if (send(sock, ptr, st.st_size, 0) < 0)
			error("Error with send.\n");
		if (munmap(ptr, st.st_size) < 0)
			error("Error with munap.\n");
		close(fd);
	}
	return (ftp_output(SENDING_SUCCESS, 0));
}

int		main(int ac, char **av)
{
	int					port;
	int					sock;

	if (ac != 3)
		usage(av[0]);
	port = atoi(av[2]);
	if ((sock = create_client(av[1], port)) < 0)
		return (-1);
	while (1)
	{
		if (!ftp_client(sock))
			break ;
	}
	close(sock);
	return (0);
}
