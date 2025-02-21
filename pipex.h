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

typedef struct s_data
{
	int		in;
	int		out;
	int		pipe[2];
	char	*paths;
	char	**cmd_paths;
	char	**cmd_args;
	char	*cmd;
	pid_t	pid1;
	pid_t	pid2;
} t_data;

void	msg_error(char *msg);
void	close_pipes(t_data *data);
char	*find_path(char *envp);
void	child_free(t_data *data);
void	parent_free(t_data *data);
void	first_child(t_data data, char *argv[], char *envp[]);
void	second_child(t_data data, char *argv[], char *envp[]);

#endif
