/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_send_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 17:41:02 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/30 19:34:47 by ysan-seb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

int		ftp_check_file_path(int sock, t_cmd cmd)
{
	int		ret;
	char	*chr;
	char	*path;
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];

	ret = 0;
	path = cmd.str + arg(cmd.str);
	chr = ft_strrchr(path, '/');
	if (chr)
		path[ft_strlen(path) - ft_strlen(chr)] = 0;
	else
		return (1);
	getcwd(old_pwd, PATH_MAX);
	if (chdir(path) < 0)
		return (ftp_request_status(sock, ERROR, 0));
	getcwd(new_pwd, PATH_MAX);
	if (ft_strstr(new_pwd, cmd.home))
		ret = 1;
	else
		ret = 0;
	chdir(old_pwd);
	if (!ret)
		ftp_request_status(sock, ERROR, 0);
	return (ret);
}

size_t	ftp_send_file_size(int sock, int fd)
{
	struct stat	st;
	char		conv[64];

	if (fstat(fd, &st) < 0)
		error("Error with fstat");
	ft_sprintf(conv, "%lld", st.st_size);
	if (send(sock, conv, ft_strlen(conv), 0) < 0)
		error("Error while sending status.");
	return (st.st_size);
}

void	ftp_send_file_content(int sock, size_t size, int fd)
{
	void	*ptr;

	if ((ptr = mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		error("Error with mmap.\n");
	if (!ftp_listen_status(sock))
		error("Error while receiving status.\n");
	if (send(sock, ptr, size, 0) < 0)
		error("Error with send.\n");
	if (munmap(ptr, size) < 0)
		error("Error with munap.\n");
	close(fd);
}

int		ftp_send_file(int sock, t_cmd cmd)
{
	int		fd;
	size_t	file_size;

	if (!ft_strcmp(cmd.str, "get") || ft_strlen(cmd.str + arg(cmd.str)) == 0)
	{
		ftp_request_status(sock, ERROR, 0);
		return (ftp_request_status(sock, MISSING_ARG, -1));
	}
	if (!ftp_check_file_path(sock, cmd))
		return (ftp_request_status(sock, INVALID_PATH_FILE, -1));
	if ((fd = ftp_open_file(sock, cmd)) < 0)
		return (ftp_request_status(sock, OPEN_ERROR, -1));
	else
		ftp_request_status(sock, SUCCESS, 0);
	file_size = ftp_send_file_size(sock, fd);
	if (file_size > 0)
		ftp_send_file_content(sock, file_size, fd);
	if (!ftp_listen_status(sock))
		error("Error while receiving status.\n");
	ftp_request_status(sock, SENDING_SUCCESS, 0);
	return (0);
}
