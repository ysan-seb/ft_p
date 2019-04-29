#include "ft_p.h"

# define MISSING_ARG "[\e[38;5;1mERROR\e[0m] Argument is missing.\n"
# define INVALID_PATH_FILE "[\e[38;5;1mERROR\e[0m] Invalid path file.\n"
# define OPEN_ERROR "[\e[38;5;1mERROR\e[0m] Can't open file.\n"
# define FSTAT_ERROR "[\e[38;5;1mERROR\e[0m] Error with fstat.\n"
# define IS_DIR "[\e[38;5;1mERROR\e[0m] Is a directory.\n"
# define IS_SYM "[\e[38;5;1mERROR\e[0m] Is a symlink.\n"
# define ERROR "0"
# define SUCCESS "1"
# define SENDING_SUCCESS "[\e[38;5;2mSUCCESS\e[0m] File has been send.\n"
# define BUFF_SIZE 1024


int		arg(char *cmd)
{
	int		i;

	i = 0;
	while (cmd[i] && !isblank(cmd[i]))
		i++;
	while (cmd[i] && isblank(cmd[i]))
		i++;
	return (i);
}

int    ftp_request_status(int sock, char *status, int ret)
{
    if (send(sock, status, strlen(status), 0) < 0)
        error("Error with send.\n");
    return (ret);
}

int     ftp_listen_status(int sock)
{
    char status[3];

    memset(&status, 0 ,3);
    if (recv(sock, status, 2, 0) < 0)
        error("Error with recv.\n");
    if (strcmp(status, "OK") == 0)
        return (1);
    return (0);
}

int     ftp_check_file_path(int sock, t_cmd cmd)
{
    int     ret;
    char    *chr;
    char    *path;
    char    old_pwd[PATH_MAX];
    char    new_pwd[PATH_MAX];

    ret = 0;
    path = cmd.str + arg(cmd.str);
    chr = strrchr(path, '/');
    if (chr)
        path[strlen(path) - strlen(chr)] = 0;
    else
        return (1);
    getcwd(old_pwd, PATH_MAX);
    if (chdir(path) < 0)
        return (ftp_request_status(sock, ERROR, 0));
    getcwd(new_pwd, PATH_MAX);
    if (strstr(new_pwd, cmd.home))
        ret = 1;
    else
        ret = 0;
    chdir(old_pwd);
    if (!ret)
        ftp_request_status(sock, ERROR, 0);
    return (ret);
}


int     ftp_open_file(int sock, t_cmd cmd)
{
    int     fd;
    struct  stat st;
    char    *path;

    path = cmd.str + arg(cmd.str);
    if ((fd = open(path, O_RDONLY)) < 0)
        return (ftp_request_status(sock, ERROR, -1));
    if (fstat(fd, &st) < 0)
        return (ftp_request_status(sock, ERROR, -1));
    if ((st.st_mode & S_IFMT) == S_IFDIR)
        return (ftp_request_status(sock, ERROR, -1));
    if ((st.st_mode & S_IFMT) == S_IFLNK)
        return (ftp_request_status(sock, ERROR, -1));
    return (fd);
}

size_t     ftp_send_file_size(int sock, int fd)
{
    struct stat st;
    char        conv[64];

    if (fstat(fd, &st) < 0)
        error("Error with fstat");
    sprintf(conv, "%lld", st.st_size);
    if (send(sock, conv, strlen(conv), 0) < 0)
        error("Error while sending status.");
    return (st.st_size);
}

void    ftp_send_file_content(int sock, int fd)
{
    int     size;
    char    buff[BUFF_SIZE];

    while ((size = read(fd, buff, BUFF_SIZE - 1)) && size != -1)
	{
		send(sock, buff, size, 0);
		memset(buff, 0, BUFF_SIZE);
	}
}

int    ftp_send_file(int sock, t_cmd cmd)
{
    int fd;
    size_t file_size;

    if (strlen(cmd.str + arg(cmd.str)) == 0)
    {
        ftp_request_status(sock, ERROR, 0);
        return (ftp_request_status(sock, MISSING_ARG, -1));
    }
    if (!ftp_check_file_path(sock, cmd))
        return (ftp_request_status(sock, INVALID_PATH_FILE, -1));
    if ((fd = ftp_open_file(sock, cmd)) < 0)
        return (ftp_request_status(sock, OPEN_ERROR, -1));
    else
        ftp_request_status(sock, SUCCESS, 0);
    if (!ftp_listen_status(sock))
        error("Error while receiving status.\n");
    file_size = ftp_send_file_size(sock, fd);
    if (!ftp_listen_status(sock))
        error("Error while receiving status.\n");
    if (file_size > 0)
        ftp_send_file_content(sock, fd);
    if (!ftp_listen_status(sock))
        error("Error while receiving status.\n");
    ftp_request_status(sock, SENDING_SUCCESS, 0);
    close (fd);
    return (0);
}
