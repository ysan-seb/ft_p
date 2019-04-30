/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client_prompt.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysan-seb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 18:14:40 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/30 18:38:23 by ysan-seb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

static t_cmd	command(int sock)
{
	t_cmd cmd;

	memset(&cmd, 0, sizeof(cmd));
	memset(&cmd.str, 0, BUFF_SIZE);
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

static int		send_command(int sock, t_cmd cmd)
{
	if (strcmp(cmd.str, "put") == 0 || strncmp(cmd.str, "put ", 4) == 0)
	{
		cmd.str[cmd.len - 1] = 0;
		return (ftp_get(sock, cmd));
	}
	else
	{
		if (send(sock, cmd.str, cmd.len, 0) < 0)
		{
			close(sock);
			exit(EXIT_SUCCESS);
		}
		if (strcmp(cmd.str, "get") == 0 || strncmp(cmd.str, "get ", 4) == 0)
			ftp_get_file(sock, cmd);
	}
	return (0);
}

int				ftp_client(int sock)
{
	t_cmd				cmd;

	memset(&cmd, 0, sizeof(cmd));
	memset(&cmd.str, 0, BUFF_SIZE);
	write(1, "ftp> ", 5);
	cmd = command(sock);
	if (cmd.len > 1 && cmd.str[0] != '\n' && !local_command(sock, cmd))
	{
		if (send_command(sock, cmd) >= 0)
		{
			memset(&cmd, 0, sizeof(cmd));
			while ((cmd.len = read(sock, cmd.str, CMD_MAX)) > 0)
			{
				write(1, cmd.str, cmd.len);
				if (strstr(cmd.str, "SUCCESS") || strstr(cmd.str, "ERROR"))
					break ;
			}
		}
	}
	return (1);
}
