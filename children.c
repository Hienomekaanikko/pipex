#include "pipex.h"

void	child_one(t_data *data, char *cmd, char **envp)
{
	data->cmd1 = ft_split(cmd, ' ');
	if (!data->cmd1)
		ft_exit(data, "malloc fail child_one", 1);
	find_path(data, data->cmd1[0], envp);
	if (dup2(data->in, 1) == -1)
		ft_exit(data, "dup2 input fail child_one", 1);
	if (dup2(data->pipe[1], 0) == -1)
		ft_exit(data, "dup2 pipe fail child_one", 1);
	close_fds(data);
	if (execve(data->path, data->cmd1, envp) < 0)
		ft_exit(data, "execve fail child_one", 1);
}

void	child_two(t_data *data, char *cmd, char **envp)
{
	data->cmd2 = ft_split(cmd, ' ');
	if (!data->cmd2)
		ft_exit(data, "malloc fail child_two", 1);
	find_path(data, data->cmd2[0], envp);
	if (dup2(data->pipe[0], 0) == -1)
		ft_exit(data, "dup2 pipe fail child_two", 1);
	if (dup2(data->out, 1) == -1)
		ft_exit(data, "dup2 output fail child_two", 1);
	close_fds(data);
	if (execve(data->path, data->cmd2, envp) < 0)
		ft_exit(data, "execve fail child_two", 1);
}
