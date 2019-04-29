/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysan-seb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 13:35:43 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/29 20:25:22 by ysan-seb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

#define CD_INV_ARG "[\e[38;5;1mERROR\e[0m] Invalid argument.\n"
#define CD_INV_DEST "[\e[38;5;1mERROR\e[0m] Invalid destination.\n"
#define CD_SUCCESS "[\e[38;5;2mSUCCESS\e[0m] Current directory has changed.\n"

void	ftp_send_msg(int client_socket, char *msg)
{
	if (send(client_socket, msg, strlen(msg), 0) < 0)
		error("Error with send.\n");
	return ;
}

void	ftp_cd_home(int client_socket, t_cmd cmd)
{
	if (chdir(cmd.home) < 0)
		error("Error with chdir.\n");
	if (send(client_socket, CD_SUCCESS, strlen(CD_SUCCESS), 0) < 0)
		error("Error with send.\n");
	return ;
}

void	command_cd(int client_socket, t_cmd cmd)
{
	int			i;
	char		path[PATH_MAX];
	char		new_path[PATH_MAX];
	static char	old_path[PATH_MAX];

	if (!strlen(old_path))
		getcwd(old_path, PATH_MAX);
	if (strcmp(cmd.str, "cd") == 0)
		return (ftp_cd_home(client_socket, cmd));
	i = arg(cmd.str);
	getcwd(path, PATH_MAX);
	if (chdir(cmd.str + i) < 0)
		return (ftp_send_msg(client_socket, CD_INV_ARG));
	getcwd(new_path, PATH_MAX);
	if (!strstr(new_path, cmd.home))
	{
		send(client_socket, CD_INV_DEST, strlen(CD_INV_DEST), 0);
		chdir(path);
		return ;
	}
	else
	{
		strcpy(old_path, path);
		send(client_socket, CD_SUCCESS, strlen(CD_SUCCESS), 0);
	}
}
