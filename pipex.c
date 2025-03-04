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
#include <unistd.h>
#include <fcntl.h>

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

void	find_path(t_data *data, char *cmd, char **envp)
{
	char	*path;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		ft_exit(data, "path not found", 127);
	data->paths = ft_split(envp[i] + 5, ':');
	if (!data->paths)
		ft_exit(data, "malloc fail find_path", 1);
	i = 0;
	while (data->paths[i])
	{
		path = ft_strjoin(data->paths[i], "/");
		if (!path)
			ft_exit(data, "malloc fail find_path", 1);
		data->path = ft_strjoin(path, cmd);
		free(path);
		if (!data->path)
			ft_exit(data, "malloc fail find_path", 1);
		if (access(data->path, F_OK | X_OK) == 0)
		{
			ft_free_split(data->paths);
			return ;
		}
		free(data->path);
		data->path = NULL;
		i++;
	}
	ft_exit(data, "path not found", 127);
}

void	child_one(t_data *data, char *cmd, char **envp)
{
	data->cmd1 = ft_split(cmd, ' ');
	if (!data->cmd1)
		ft_exit(data, "malloc fail child_one", 1);
	find_path(data, data->cmd1[0], envp);
	if (dup2(data->in, STDIN_FILENO) == -1)
		ft_exit(data, "dup2 input fail child_one", 1);
	if (dup2(data->pipe[1], STDOUT_FILENO) == -1)
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
	if (dup2(data->pipe[0], STDIN_FILENO) == -1)
		ft_exit(data, "dup2 pipe fail child_two", 1);
	if (dup2(data->out, STDOUT_FILENO) == -1)
		ft_exit(data, "dup2 output fail child_two", 1);
	close_fds(data);
	if (execve(data->path, data->cmd2, envp) < 0)
		ft_exit(data, "execve fail child_two", 1);
}

int	main(int argc, char **argv, char **envp)
{
	int		status;
	t_data	data;

	if (argc != 5)
		ft_exit(&data, "usage: ./pipex infile cmd1 cmd2 outfile", 1);
	init_data(&data);
	prep_env(argv, &data);
	data.pid1 = fork();
	if (data.pid1 == -1)
		ft_exit(&data, "pid1 fork fail", 1);
	else if (data.pid1 == 0)
		child_one(&data, argv[2], envp);
	data.pid2 = fork();
	if (data.pid2 == -1)
		ft_exit(&data, "pid2 fork fail", 1);
	else if (data.pid2 == 0)
		child_two(&data, argv[3], envp);
	close_fds(&data);
	if (waitpid(data.pid1, &status, 0) == -1)
		ft_exit(&data, "waitpid fail child_one", 1);
	if (WIFEXITED(status) && WEXITSTATUS(status) > 1)
		ft_exit(&data, "child process 1 exited with error", WEXITSTATUS(status));
	if (waitpid(data.pid2, &status, 0) == -1)
		ft_exit(&data, "waitpid fail child_two", 1);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		ft_exit(&data, "child process 2 exited with error", WEXITSTATUS(status));
	ft_exit(&data, NULL, 0);
	return (0);
}
