#include "pipex.h"

char	*find_path(char *envp)
{
	while (ft_strncmp("PATH", *envp, 4))
		envp++;
	return (*envp + 5);
}
void	close_pipes(t_data *data)
{
	close(data->pipe[0]);
	close(data->pipe[1]);
}

void	parent_free(t_data *data)
{
	int	i;

	i = 0;
	close(data->in);
	close(data->out);
	while (data->cmd_paths[i])
	{
		free(data->cmd_paths[i]);
		i++;
	}
	free(data->cmd_paths);
}

void	child_free(t_data *data)
{
	int	i;

	i = 0;
	while (data->cmd_args[i])
	{
		free(data->cmd_args[i]);
		i++;
	}
	free(data->cmd_args);
	free(data->cmd);
}
