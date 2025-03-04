#include "pipex.h"

void	prep_env(char **argv, t_data *data)
{
	if (access(argv[1], R_OK) == -1)
		ft_exit(data, "input file not readable", 1);
	data->in = open(argv[1], O_RDONLY);
	if (data->in == -1)
		ft_exit(data, "input file fail prep_env", 1);
	data->out = open(argv[4], O_TRUNC | O_CREAT | O_RDWR, 0644);
	if (data->out == -1)
		ft_exit(data, "output file fail prep_env", 1);
	if (pipe(data->pipe) == -1)
		ft_exit(data, "pipe fail prep_env", 1);
}

void	init_data(t_data *data)
{
	data->path = NULL;
	data->paths = NULL;
	data->cmd1 = NULL;
	data->cmd2 = NULL;
	data->in = -1;
	data->out = -1;
	data->pipe[0] = -1;
	data->pipe[1] = -1;
}
