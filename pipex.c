/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:42:49 by msuokas           #+#    #+#             */
/*   Updated: 2025/02/27 17:43:15 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_data(t_data *data)
{
	data->paths = NULL;
	data->path = NULL;
	data->cmd1 = NULL;
	data->cmd2 = NULL;
	data->in = -1;
	data->out = -1;
	data->pipe[0] = -1;
	data->pipe[1] = -1;
}

void	ft_exit(t_data *data, char *msg, int exitcode)
{
	if (data->paths)
		ft_free_split(data->paths);
	if (data->path)
		free(data->path);
	if (data->cmd1)
		ft_free_split(data->cmd1);
	if (data->cmd2)
		ft_free_split(data->cmd2);
	if (data->in != -1)
		close(data->in);
	if (data->out != -1)
		close(data->out);
	if (data->pipe[0] != -1)
		close(data->pipe[0]);
	if (data->pipe[1] != -1)
		close(data->pipe[1]);
	if (msg != NULL)
		perror(msg);
	exit(exitcode);
}

void	get_path(t_data *data, char *command, char **envp)
{
	int		i;
	char	*path;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		ft_exit(data, "path not found", 127);
	data->paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (data->paths[i])
	{
		path = ft_strjoin(data->paths[i], "/");
		data->path = ft_strjoin(path, command);
		free(path);
		if (access(data->path, F_OK | X_OK) == 0)
		{
			ft_free_split(data->paths);
			return ;
		}
		free(data->path);
		data->path = NULL;
		i++;
	}
	ft_free_split(data->paths);
	data->paths = NULL;
	ft_exit(data, "command not found", 127);
}

void	prep_env(t_data *data, int argc, char **argv)
{
	data->in = open(argv[1], O_RDONLY);
	if (data->in < 0)
		perror("Input file error");
	if ((data->out = open(argv[argc - 1], O_TRUNC | O_CREAT | O_RDWR, 0644)) < 0)
	{
		if (data->in >= 0)
			close(data->in);
		ft_exit(data, "output fail", 1);
	}
	if (pipe(data->pipe) < 0)
	{
		if (data->in >= 0)
			close(data->in);
		close(data->out);
		ft_exit(data, "pipe failed", 1);
	}
}

void	child_one(t_data *data, char *command, char **envp)
{
	if (!command[0])
		ft_exit(data, "Empty command", 1);
	data->cmd1 = ft_split(command, ' ');
	if (!data->cmd1)
		ft_exit(data, "Memory allocation failed", 1);
	get_path(data, data->cmd1[0], envp);
	if (data->in >= 0)
		dup2(data->in, 0);
	dup2(data->pipe[1], 1);
	close(data->pipe[0]);
	close(data->pipe[1]);
	if (data->in >= 0)
		close(data->in);
	close(data->out);
	if (data->in < 0 || execve(data->path, data->cmd1, envp) == -1)
		ft_exit(data, "Execve failed in child one", 1);
}

void	child_two(t_data *data, char *command, char **envp)
{
	if (!command[0])
		ft_exit(data, "Empty command", 1);
	data->cmd2 = ft_split(command, ' ');
	if (!data->cmd2)
		ft_exit(data, "Memory allocation failed", 1);
	get_path(data, data->cmd2[0], envp);
	dup2(data->pipe[0], 0);
	dup2(data->out, 1);
	close(data->pipe[0]);
	close(data->pipe[1]);
	close(data->in);
	close(data->out);
	if (execve(data->path, data->cmd2, envp) == -1)
		ft_exit(data, "Execve failed in child two", 1);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	int		status1;
	int		status2;

	if (argc != 5)
	{
		ft_putendl_fd("Usage: ./pipex infile cmd1 cmd2 outfile", 2);
		exit(1);
	}
	init_data(&data);
	prep_env(&data, argc, argv);
	data.pid1 = fork();
	if (data.pid1 == 0)
		child_one(&data, argv[2], envp);
	data.pid2 = fork();
	if (data.pid2 == 0)
		child_two(&data, argv[3], envp);
	close(data.pipe[0]);
	close(data.pipe[1]);
	if (data.in >= 0)
		close(data.in);
	close(data.out);
	waitpid(data.pid1, &status1, 0);
	waitpid(data.pid2, &status2, 0);
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	else if (WIFEXITED(status1))
		return (WEXITSTATUS(status1));
	return (1);
}
