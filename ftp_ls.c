/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_ls.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysan-seb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 14:19:39 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/27 14:21:13 by ysan-seb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

#define LS_ERROR "[\e[38;5;1mERROR\e[0m] Invalid argument.\n"
#define LS_SUCCESS "[\e[38;5;2mSUCCESS\e[0m] Directory has been listed.\n"

void	command_ls(int client_socket, t_cmd cmd)
{
	int		i;
	pid_t	pidchild;
	char	arg[64];

	memset(&arg, 0, 64);
	i = get_argument(cmd.str);
	if (cmd.str[i] == '-')
		strncat(arg, cmd.str + i, 64);
	else if (cmd.str[i] == '\0')
		strncat(arg, ".", 64);
	else
	{
		send(client_socket, LS_ERROR, strlen(LS_ERROR), 0);
		return ;
	}
	if ((pidchild = fork()) == 0)
	{
		dup2(client_socket, 1);
		dup2(client_socket, 2);
		execl("/bin/ls", "ls", arg, NULL);
		exit(EXIT_SUCCESS);
	}
	else if (pidchild > 0)
		wait(&pidchild);
	send(client_socket, LS_SUCCESS, strlen(LS_SUCCESS), 0);
}
