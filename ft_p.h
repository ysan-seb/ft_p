#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define CMD_MAX 1024
// #define PATH_MAX 1024

#define ERROR "[\e[38;5;1mERROR\e[0m]\n"
#define SUCCESS "[\e[38;5;2mSUCCESS\e[0m]\n"

typedef struct			s_client
{
	int					cs;
	unsigned int		cslen;
	struct sockaddr_in	csin;
}						t_client;

typedef struct	s_cmd
{
	char		str[CMD_MAX];
    char        home[PATH_MAX];
	int			len;
}				t_cmd;

int		get_argument(char *cmd);

void	usage(char *bin);
void	error(char *err);
void	command_ls(int client_socket, t_cmd cmd);
void	command_cd(int client_socket, t_cmd cmd);
void	command_pwd(int client_socket);