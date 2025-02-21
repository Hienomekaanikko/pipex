#include "pipex.h"

void	msg_error(char *msg)
{
	perror(msg);
	exit (1);
}
