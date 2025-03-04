#include "pipex.h"

void	close_fds(t_data *data)
{
	if (data->in != -1)
		close(data->in);
	if (data->out != -1)
		close(data->out);
	if (data->pipe[0] != -1)
		close(data->pipe[0]);
	if (data->pipe[1] != -1)
		close(data->pipe[1]);
}

void	ft_exit(t_data *data, char *msg, int exitcode)
{
	close_fds(data);
	if (data->path)
		free(data->path);
	if (data->paths)
		ft_free_split(data->paths);
	if (data->cmd1)
		ft_free_split(data->cmd1);
	if (data->cmd2)
		ft_free_split(data->cmd2);
	if (msg)
		perror(msg);
	exit(exitcode);
}
