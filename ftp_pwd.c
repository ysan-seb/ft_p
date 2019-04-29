/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysan-seb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 14:19:02 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/29 17:48:21 by ysan-seb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

#define PWD_SUCCESS "\n[\e[38;5;2mSUCCESS\e[0m] Pwd has been executed.\n"
#define PWD_ERROR "\n[\e[38;5;1mERROR\e[0m] Can't execute pwd.\n"

void	command_pwd(int client_socket)
{
	char	path[PATH_MAX];

	getcwd(path, PATH_MAX);
	if (strlen(path) == 0)
		send(client_socket, PWD_ERROR, strlen(PWD_ERROR), 0);
	else
	{
		send(client_socket, path, strlen(path), 0);
		send(client_socket, PWD_SUCCESS, strlen(PWD_SUCCESS), 0);
	}
}
