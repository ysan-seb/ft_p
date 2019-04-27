/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysan-seb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 13:35:43 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/27 13:36:01 by ysan-seb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

# define CD_INV_ARG "[\e[38;5;1mERROR\e[0m] Invalid argument.\n"
# define CD_INV_DEST "[\e[38;5;1mERROR\e[0m] Invalid destination.\n"
# define CD_SUCCESS "[\e[38;5;2mSUCCESS\e[0m] Current directory has been changed.\n"

void	command_cd(int client_socket, t_cmd cmd)
{
	int 		i;
	char		path[PATH_MAX];
	char		new_path[PATH_MAX];
	static char	old_path[PATH_MAX];

	if (!strlen(old_path))
		getcwd(old_path, PATH_MAX);
	if (strcmp(cmd.str, "cd") == 0)
	{
		chdir(cmd.home);
		send(client_socket, CD_SUCCESS, strlen(CD_SUCCESS), 0);
		return;
	}
	i = get_argument(cmd.str);
	getcwd(path, PATH_MAX);
	if (chdir(cmd.str + i) < 0)
	{
		send(client_socket, CD_INV_ARG, strlen(CD_INV_ARG), 0);
		return ;
	}
	getcwd(new_path, PATH_MAX);
	if (!strstr(new_path, cmd.home))
	{
		send(client_socket, CD_INV_DEST, strlen(CD_INV_DEST), 0);
		chdir(path);
		return ;
	} 
	else {
		strcpy(old_path, path);
		send(client_socket, CD_SUCCESS, strlen(CD_SUCCESS), 0);
	}
}
