#include "ft_p.h"

void	command_cd(int client_socket, t_cmd cmd)
{
	int 		i;
	char		path[PATH_MAX];
	char		new_path[PATH_MAX];
	static char	old_path[PATH_MAX];

	if (!strlen(old_path))
		getcwd(old_path, PATH_MAX);
	if (strcmp(cmd.str, "cd") == 0)
	{
		chdir(cmd.home);
		send(client_socket, SUCCESS, strlen(SUCCESS), 0);
		return;
	}
	i = get_argument(cmd.str);
	getcwd(path, PATH_MAX);
	if (chdir(cmd.str + i) < 0)
	{
		send(client_socket, "?Invalid argument.", 17, 0);
		return ;
	}
	getcwd(new_path, PATH_MAX);
	if (!strstr(new_path, cmd.home))
	{
		send(client_socket, ERROR, strlen(ERROR), 0);
		chdir(path);
		return ;
	} 
	else {
		strcpy(old_path, path);
		send(client_socket, SUCCESS, strlen(SUCCESS), 0);
	}
}