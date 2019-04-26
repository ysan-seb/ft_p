#include "ft_p.h"

void	error(char *err)
{
	dprintf(2, "%s", err);
	exit(-1);
}