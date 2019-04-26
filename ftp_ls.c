#include "ft_p.h"

void	command_ls(int client_socket, t_cmd cmd)
{
	int 	i;
	pid_t 	pidchild;
	char	arg[64];

	memset(&arg, 0, 64);
	i = get_argument(cmd.str);
	if (cmd.str[i] == '-')
		strncat(arg, cmd.str + i, 64);
	else if (cmd.str[i] == '\0')
		strncat(arg, ".", 64);
	else
	{
		send(client_socket, "?Invalid argument.", 17, 0);
		return;		
	}
	if ((pidchild = fork()) == 0)
	{
		dup2(client_socket, 1);
		dup2(client_socket, 2);
		execl("/bin/ls", "ls", arg, NULL);
		exit(EXIT_SUCCESS);
	}
	else if (pidchild > 0)
		wait(&pidchild);
}