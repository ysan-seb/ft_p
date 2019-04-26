#include "ft_p.h"

void	command_pwd(int client_socket)
{
	char	path[PATH_MAX];

	getcwd(path, PATH_MAX);
	send(client_socket, path, strlen(path), 0);
}