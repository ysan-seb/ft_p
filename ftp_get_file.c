/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_get_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 18:06:54 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/30 00:03:34 by maki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

#define BUFF_SIZE 1024

char	*filename(char *cmd)
{
	char	*chr;
	char	*filename;

	chr = strrchr(cmd, '/');
	if (!chr)
		filename = cmd;
	else
		filename = cmd + (strlen(cmd) - strlen(chr)) + 1;
	return (filename);
}

int		ftp_get_file_size(int sock)
{
	size_t	size;
	char	conv[64];

	if (recv(sock, conv, 64, 0) < 0)
		error("Error with recv.\n");
	size = atoi(conv);
	return (size);
}

void	ftp_get_file_content(int sock, t_file file)
{
	int		size;
	size_t	tmp;

	tmp = 0;
	while ((size = recv(sock, file.content, BUFF_SIZE - 1, 0)) && size != -1)
	{
		tmp += size;
		write(file.fd, file.content, size);
		if (tmp == file.size)
			break ;
	}
}

void		ftp_get_file(int sock, t_cmd cmd)
{
	t_file	file;

    cmd.str[cmd.len - 1] = 0;
	if (!ftp_get_file_header(sock))
		return ;
	file.name = filename(cmd.str + arg(cmd.str));
	if ((file.fd = open(file.name, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
		error("Error with open.\n");
	if (send(sock, "OK", 2, 0) < 0)
		error("Error with send.\n");
	if ((file.size = ftp_get_file_size(sock)))
    {
        if (send(sock, "OK", 2, 0) < 0)
            error("Error with send.\n");
		ftp_get_file_content(sock, file);
    }
	if (send(sock, "OK", 2, 0) < 0)
		error("Error with send.\n");
	close(file.fd);
}
