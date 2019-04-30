/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_request_status.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysan-seb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 19:41:38 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/29 19:41:45 by ysan-seb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

int		ftp_request_status(int sock, char *status, int ret)
{
	if (send(sock, status, ft_strlen(status), 0) < 0)
		error("Error with send.\n");
	return (ret);
}
