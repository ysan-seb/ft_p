/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_ls.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 14:19:39 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/29 23:55:18 by maki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

void	command_ls(int client_socket, t_cmd cmd)
{
	int		i;
	int		status;
	pid_t	pidchild;
	char	opt[64];

	ft_memset(&opt, 0, 64);
	i = arg(cmd.str);
	if (cmd.str[i] == '-')
		strncat(opt, cmd.str + i, 64);
	else if (cmd.str[i] == '\0')
		strncat(opt, ".", 64);
	else
	{
		send(client_socket, LS_ERROR, ft_strlen(LS_ERROR), 0);
		return ;
	}
	if ((pidchild = fork()) == 0)
	{
		dup2(client_socket, 1);
		dup2(client_socket, 2);
		execl("/bin/ls", "ls", opt, NULL);
	}
	else if (pidchild > 0)
		wait4(pidchild, &status, 0, 0);
	send(client_socket, LS_SUCCESS, ft_strlen(LS_SUCCESS), 0);
}
