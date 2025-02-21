#include "pipex.h"

static char	*get_cmd(char **paths, char *cmd)
{
	char	*tmp;
	char	*command;

	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/");
		command = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(command, 0) == 0)
			return (command);
		free(command);
		paths++;
	}
	return (NULL);
}

void	first_child(t_data data, char *argv[], char *envp[])
{
	dup2(data.pipe[1], 1);
	close(data.pipe[0]);
	dup2(data.in, 0);
	data.cmd_args = ft_split(argv[2], ' ');
	data.cmd = get_cmd(data.cmd_paths, data.cmd_args[0]);
	if (!data.cmd)
	{
		child_free(&data);
		msg_error("Command not found");
		exit(1);
	}
	execve(data.cmd, data.cmd_args, envp);
}

void	second_child(t_data data, char *argv[], char *envp[])
{
	dup2(data.pipe[0], 0);
	close(data.pipe[1]);
	dup2(data.out, 1);
	data.cmd_args = ft_split(argv[3], ' ');
	data.cmd = get_cmd(data.cmd_paths, data.cmd_args[0]);
	if (!data.cmd)
	{
		child_free(&data);
		msg_error("Command not found");
		exit(1);
	}
	execve(data.cmd, data.cmd_args, envp);
}
