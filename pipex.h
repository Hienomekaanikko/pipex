#ifndef PIPEX_H
#define PIPEX_H

# include <unistd.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "libft/libft.h"

# define ERR_INFILE "Infile"
# define ERR_OUTFILE "Outfile"
# define ERR_PIPE "Pipe"
# define ERR_CMD "Command not found\n"

typedef struct s_data
{
	int	in;
	int	out;
	int	pipe[2];
	char *paths;
} t_data;

void	msg_error(char *msg);

#endif
