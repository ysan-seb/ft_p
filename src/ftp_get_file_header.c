/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_get_file_header.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysan-seb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 17:35:40 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/30 17:35:42 by ysan-seb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

int		ftp_get_file_header(int sock)
{
	char	status[2];

	ft_memset(&status, 0, 2);
	if (recv(sock, status, 1, 0) < 0)
		error("Error with recv.\n");
	if (status[0] == '0')
		return (0);
	return (1);
}
