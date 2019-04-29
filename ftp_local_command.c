/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_local_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysan-seb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 20:29:00 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/29 20:29:06 by ysan-seb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

int		local_command(int sock, t_cmd cmd)
{
	if (strcmp(cmd.str, "quit\n") == 0)
	{
		send(sock, "quit\n", 5, 0);
		printf("\e[3mDisconnected from server.\e[0m\n");
		close(sock);
		exit(EXIT_SUCCESS);
	}
	return (0);
}
