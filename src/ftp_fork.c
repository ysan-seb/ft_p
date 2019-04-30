/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysan-seb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 18:25:12 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/30 18:25:52 by ysan-seb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

int		ftp_fork(int sock)
{
	pid_t		child;
	t_cmd		cmd;
	t_client	c;

	getcwd(cmd.home, PATH_MAX);
	if ((c.cs = accept(sock, (struct sockaddr*)&c.csin, &c.cslen)) < 0)
		return (0);
	if ((child = fork()) == 0)
	{
		while (1)
		{
			ft_memset(&cmd.str, 0, PATH_MAX);
			if ((cmd.len = recv(c.cs, cmd.str, CMD_MAX - 1, 0)) < 0)
			{
				close(c.cs);
				return (0);
			}
			cmd.str[cmd.len] = '\0';
			builtins(c.cs, cmd);
		}
	}
	return (1);
}
