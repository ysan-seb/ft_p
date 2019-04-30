#include "ft_p.h"

int		ftp_get_file_header(int sock)
{
	char	status[2];

	memset(&status, 0, 2);
	if (recv(sock, status, 1, 0) < 0)
		error("Error with recv.\n");
	if (status[0] == '0')
		return (0);
	return (1);
}