/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client_prompt.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysan-seb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 18:14:40 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/30 19:33:21 by ysan-seb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

static t_cmd	command(int sock)
{
	t_cmd cmd;

	ft_memset(&cmd, 0, sizeof(cmd));
	ft_memset(&cmd.str, 0, BUFF_SIZE);
	if ((cmd.len = read(0, &cmd.str, CMD_MAX - 1)) < 0)
	{
		close(sock);
		exit(EXIT_FAILURE);
	}
	else if (!cmd.len)
	{
		close(sock);
		ft_printf("\e[3mDisconnected from server.\e[0m\n");
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
	if (!ft_strcmp(cmd.str, "put") || !ft_strncmp(cmd.str, "put ", 4))
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
		if (!ft_strcmp(cmd.str, "get") || !ft_strncmp(cmd.str, "get ", 4))
			ftp_get_file(sock, cmd);
	}
	return (0);
}

int				ftp_client(int sock)
{
	t_cmd				c;

	ft_memset(&c, 0, sizeof(c));
	ft_memset(&c.str, 0, BUFF_SIZE);
	write(1, "ftp> ", 5);
	c = command(sock);
	if (c.len > 1 && c.str[0] != '\n' && !local_command(sock, c))
	{
		if (send_command(sock, c) >= 0)
		{
			ft_memset(&c, 0, sizeof(c));
			while ((c.len = read(sock, c.str, CMD_MAX)) > 0)
			{
				write(1, c.str, c.len);
				if (ft_strstr(c.str, "SUCCESS") || ft_strstr(c.str, "ERROR"))
					break ;
			}
		}
	}
	return (1);
}
