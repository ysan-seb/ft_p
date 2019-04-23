/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysan-seb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 21:48:04 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/23 23:27:02 by ysan-seb         ###   ########.fr       */
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

void	usage(char *bin)
{
	printf("Usage: %s <addr> <port>\n", bin);
	exit(-1);
}

void	error(char *err)
{
	dprintf(2, "%s\n", err);
	exit(1);
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
		error("[\e[38;5;1m-\e[0m]Error in connection.");
	printf("[\e[38;5;2m+\e[0m]Client socket is created.\n");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(addr);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) < 0)
		error("[\e[38;5;1m-\e[0m]Error in connection.");
	printf("[\e[38;5;2m+\e[0m]Connected to Server.\n");
	return (sock);
}

int		main(int ac, char **av)
{
	int					port;
	int					sock;
	int					r;
	char				buf[1024];

	if (ac != 3)
		usage(av[0]);
	port = atoi(av[2]);
	sock = create_client(av[1], port);
	while (1)
	{
		write(1, "ftp> ", 5);
		if ((r = read(0, &buf, 1023)) < 0)
			break;
		if (r == 0)
		{
			close(sock);
			printf("\e[3mDisconnected from server.\n");
			exit(1);
		}
		buf[r] = '\0';
		if (strlen(buf) > 0)
		{
			if (send(sock, buf, strlen(buf), 0) < 0)
				break;;
			if (recv(sock, buf, 1023, 0) < 0)
				printf("[\e[38;5;1m-\e[0m]Error in receiving data.\n");
			else
			{
				if (buf[strlen(buf) - 1] != '\n')
					printf("%s\n", buf);
				else
					printf("%s", buf);
			}
		}
	}
	close(sock);
	return (0);
}
