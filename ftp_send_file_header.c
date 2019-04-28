#include "ft_p.h"

# define INVALID_PATH_FILE "[\e[38;5;1mERROR\e[0m] Invalid path file.\n"
# define OPEN_ERROR "[\e[38;5;1mERROR\e[0m] Can't open file.\n"
# define FSTAT_ERROR "[\e[38;5;1mERROR\e[0m] Error with fstat.\n"
# define IS_DIR "[\e[38;5;1mERROR\e[0m] Is a directory.\n"
# define IS_SYM "[\e[38;5;1mERROR\e[0m] Is a symlink.\n"
# define ERROR "0"
# define SUCCESS "1"
# define SENDING_SUCCESS "[\e[38;5;2mSUCCESS\e[0m] File has been send.\n"


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
    printf("%s\n", status);
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
    char    old_pwd[PATH_MAX];
    char    new_pwd[PATH_MAX];

    ret = 0;
    chr = strrchr(cmd.str, '/');
    if (chr)
        cmd.str[strlen(cmd.str) - strlen(chr)] = 0;
    getcwd(old_pwd, PATH_MAX);
    chdir(cmd.str);
    getcwd(new_pwd, PATH_MAX);
    if (strstr(new_pwd, cmd.home))
        ret = 1;
    else
        ret = 0;
    chdir(old_pwd);
    printf("%s %s ret : %d\n", new_pwd, cmd.home, ret);
    if (!ret)
        ftp_request_status(sock, ERROR, -1);
    return (ret);
}


int     ftp_open_file(int sock, t_cmd cmd)
{
    int     fd;
    struct  stat st;
    char    *path;

    path = cmd.str + arg(cmd.str);
    printf("path = %s\n", path);
    if ((fd = open(path, O_RDONLY)) < 0)
        return (ftp_request_status(sock, ERROR, -1));
    if (fstat(fd, &st) < 0)
        return (ftp_request_status(sock, ERROR, -1));
    if ((st.st_mode & S_IFMT) == S_IFDIR)
        return (ftp_request_status(sock, ERROR, -1));
    if ((st.st_mode & S_IFMT) == S_IFLNK)
        return (ftp_request_status(sock, ERROR, -1));
    printf("fd : %d -\n", fd);
    return (fd);
}

size_t     ftp_send_file_size(int sock, int fd)
{
    struct stat st;
    char        conv[64];

    if (fstat(fd, &st) < 0)
        error("Error with fstat");
    sprintf(conv, "%ld", st.st_size);
    if (send(sock, conv, strlen(conv), 0) < 0)
        error("Error while sending status.");
    return (st.st_size);
}

void    ftp_send_file_content(int sock, int fd, size_t size)
{
    char *content;
    
    if (!(content = (char*)malloc(sizeof(char) * (size + 1))))
        error("Error with malloc.\n");
    printf("%p\n", content);
	if (content)
    {
        if (read(fd, content, size) < 0)
            error("Error with read.\n");
        printf("ptr = %p\n", content);
		if (send(sock, content, size, 0) < 0)
            error("Error while sending file content.\n");
        free(content);
    }
}

int    ftp_send_file(int sock, t_cmd cmd)
{
    int fd;
    size_t file_size;

    // if (!ftp_check_file_path(sock, cmd))
    //     return (ftp_request_status(sock, INVALID_PATH_FILE, -1));
    if ((fd = ftp_open_file(sock, cmd)) < 0)
        return (ftp_request_status(sock, OPEN_ERROR, -1));
    else
        ftp_request_status(sock, SUCCESS, 0);
    if (!ftp_listen_status(sock))
        error("Error while receiving status.\n");
    file_size = ftp_send_file_size(sock, fd);
    if (!ftp_listen_status(sock))
        error("Error while receiving status.\n");
    printf("fd = %d size %zu\n", fd, file_size);
    if (file_size > 0)
        ftp_send_file_content(sock, fd, file_size);
    if (!ftp_listen_status(sock))
        error("Error while receiving status.\n");
    ftp_request_status(sock, SENDING_SUCCESS, 0);
    close (fd);
    return (0);
}