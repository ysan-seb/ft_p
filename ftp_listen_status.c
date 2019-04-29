/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_listen_status.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysan-seb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 19:40:00 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/29 19:40:03 by ysan-seb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

int		ftp_listen_status(int sock)
{
	char	status[3];

	memset(&status, 0, 3);
	if (recv(sock, status, 2, 0) < 0)
		error("Error with recv.\n");
	if (strcmp(status, "OK") == 0)
		return (1);
	return (0);
}
