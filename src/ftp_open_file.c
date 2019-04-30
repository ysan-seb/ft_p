/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_open_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysan-seb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 17:36:34 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/30 17:36:39 by ysan-seb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

int		ftp_open_file(int sock, t_cmd cmd)
{
	int			fd;
	struct stat	st;
	char		*path;

	path = cmd.str + arg(cmd.str);
	if ((fd = open(path, O_RDONLY)) < 0)
		return (ftp_request_status(sock, ERROR, -1));
	if (fstat(fd, &st) < 0)
		return (ftp_request_status(sock, ERROR, -1));
	if ((st.st_mode & S_IFMT) == S_IFDIR)
		return (ftp_request_status(sock, ERROR, -1));
	if ((st.st_mode & S_IFMT) == S_IFLNK)
		return (ftp_request_status(sock, ERROR, -1));
	return (fd);
}
