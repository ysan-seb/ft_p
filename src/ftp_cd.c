/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 13:35:43 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/29 23:54:26 by maki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

void	ftp_send_msg(int client_socket, char *msg)
{
	if (send(client_socket, msg, ft_strlen(msg), 0) < 0)
		error("Error with send.\n");
	return ;
}

void	ftp_cd_home(int client_socket, t_cmd cmd)
{
	if (chdir(cmd.home) < 0)
		error("Error with chdir.\n");
	if (send(client_socket, CD_SUCCESS, ft_strlen(CD_SUCCESS), 0) < 0)
		error("Error with send.\n");
	return ;
}

void	command_cd(int client_socket, t_cmd cmd)
{
	int			i;
	char		path[PATH_MAX];
	char		new_path[PATH_MAX];
	static char	old_path[PATH_MAX];

	if (!ft_strlen(old_path))
		getcwd(old_path, PATH_MAX);
	if (ft_strcmp(cmd.str, "cd") == 0)
		return (ftp_cd_home(client_socket, cmd));
	i = arg(cmd.str);
	getcwd(path, PATH_MAX);
	if (chdir(cmd.str + i) < 0)
		return (ftp_send_msg(client_socket, CD_INV_ARG));
	getcwd(new_path, PATH_MAX);
	if (!ft_strstr(new_path, cmd.home))
	{
		send(client_socket, CD_INV_DEST, ft_strlen(CD_INV_DEST), 0);
		chdir(path);
		return ;
	}
	else
	{
		strcpy(old_path, path);
		send(client_socket, CD_SUCCESS, ft_strlen(CD_SUCCESS), 0);
	}
}
