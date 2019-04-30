/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 14:19:02 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/29 23:55:39 by maki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

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
