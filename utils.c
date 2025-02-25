#include "pipex.h"

void	prep_env(t_data *data, int argc, char **argv)
{
	data->in = open(argv[1], O_RDONLY);
	if (data->in < 0)
	{
		perror("Failed to open input file");
		exit(1);
	}
	data->out = open(argv[argc - 1], O_TRUNC | O_CREAT | O_RDWR, 0000644);
	if (data->out < 0)
	{
		perror("Failed to open output file");
		exit(1);
	}
	if (pipe(data->pipe) < 0)
	{
		perror("Pipe failed");
		exit(1);
	}
}

void	init_child_one(t_data *data, char **argv, char **envp)
{
	if (data->pid1 == -1)
	{
		perror("Fork failed for pid1");
		exit(1);
	}
	if (data->pid1 == 0)
		child_one(data, argv, envp);
}

void	init_child_two(t_data *data, char **argv, char **envp)
{
	if (data->pid2 == -1)
	{
		perror("Fork failed for pid2");
		exit(1);
	}
	if (data->pid2 == 0)
		child_two(data, argv, envp);
}

void	get_path(t_data *data, char *cmd, char **envp)
{
	int		i;
	char	*dir_path;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (envp[i] == NULL)
	{
		fprintf(stderr, "PATH variable not found\n");
		return ;
	}
	data->paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (data->paths[i])
	{
		dir_path = ft_strjoin(data->paths[i], "/");
		data->path = ft_strjoin(dir_path, cmd);
		free(dir_path);
		if (access(data->path, F_OK | X_OK) == 0)
			return ;
		free(data->path);
		data->path = NULL;
		i++;
	}
	fprintf(stderr, "Command not found in path: %s\n", cmd);
}

void	child_one(t_data *data, char **argv, char **envp)
{
	char	**cmd_args;

	dup2(data->in, STDIN_FILENO);
	dup2(data->pipe[1], STDOUT_FILENO);
	close(data->pipe[0]);
	close(data->pipe[1]);
	close(data->in);
	close(data->out);

	get_path(data, ft_split(argv[2], ' ')[0], envp);
	if (data->path == NULL)
	{
		fprintf(stderr, "Command not found: %s\n", argv[2]);
		exit(1);
	}
	cmd_args = ft_split(argv[2], ' ');
	execve(data->path, cmd_args, envp);
	perror("Execve failed in child one");
	ft_free_split(cmd_args);
	exit(1);
}

void	child_two(t_data *data, char **argv, char **envp)
{
	char	**cmd_args;

	dup2(data->pipe[0], STDIN_FILENO);
	dup2(data->out, STDOUT_FILENO);
	close(data->pipe[0]);
	close(data->pipe[1]);
	close(data->in);
	close(data->out);

	get_path(data, ft_split(argv[3], ' ')[0], envp);
	if (data->path == NULL)
	{
		fprintf(stderr, "Command not found: %s\n", argv[3]);
		exit(1);
	}
	cmd_args = ft_split(argv[3], ' ');
	execve(data->path, cmd_args, envp);
	perror("Execve failed in child two");
	ft_free_split(cmd_args);
	exit(1);
}
