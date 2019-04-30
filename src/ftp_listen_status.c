/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_listen_status.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 19:40:00 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/30 00:38:38 by maki             ###   ########.fr       */
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
