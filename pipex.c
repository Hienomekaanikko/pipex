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

void	ft_exit(t_data *data, char *msg, int exitcode)
{
	if (data->in != -1)
		close(data->in);
	if (data->out != -1)
		close(data->out);
	if (data->pipe[0] != -1)
		close(data->pipe[0]);
	if (data->pipe[1] != -1)
		close(data->pipe[1]);
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
	data->in = open(argv[1], O_RDONLY);
	data->out = open(argv[4], O_TRUNC | O_CREAT | O_RDWR, 0644);
	pipe(data->pipe);
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
		ft_exit(data, "ERROR: Path not found", 127);
	data->paths = ft_split(envp[i] + 5, ':');
	if (!data->paths)
		ft_exit(data, "ERROR: Malloc", 1);
	i = 0;
	while (envp[i])
	{
		path = ft_strjoin(data->paths[i], "/");
		if (!path)
			ft_exit(data, "ERROR: Malloc", 1);
		data->path = ft_strjoin(path, cmd);
		if (!data->path)
			ft_exit(data, "ERROR: Malloc", 1);
		if (access(data->path, F_OK | X_OK) == 0)
		{
			ft_free_split(data->paths);
			return ;
		}
		free(data->path);
		data->path = NULL;
		i++;
	}
	ft_exit(data, "ERROR: Path not found", 127);
}

void	child_one(t_data *data, char *cmd, char **envp)
{
	data->cmd1 = ft_split(cmd, ' ');
	find_path(data, data->cmd1[0], envp);
	if (!data->cmd1)
		ft_exit(data, "ERROR: Malloc", 1);
	dup2(data->in, 0);
	close(data->in);
	dup2(data->pipe[1], 1);
	close(data->pipe[1]);
	execve(data->path, data->cmd1, envp);
}

void	child_two(t_data *data, char *cmd, char **envp)
{
	data->cmd2 = ft_split(cmd, ' ');
	find_path(data, data->cmd2[0], envp);
	if (!data->cmd2)
		ft_exit(data, "ERROR: Malloc", 1);
	dup2(data->out, 1);
	close(data->out);
	dup2(data->pipe[0], 0);
	close(data->pipe[0]);
	execve(data->path, data->cmd2, envp);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	init_data(&data);
	if (argc != 5)
		ft_exit(&data, "ERROR: Invalid amount of arguments", 1);
	else
	{
		prep_env(argv, &data);
		data.pid1 = fork();
		if (data.pid1 == -1)
			ft_exit(&data, "ERROR: Fork", 1);
		else if (data.pid1 == 0)
			child_one(&data, argv[2], envp);
		data.pid2 = fork();
		if (data.pid2 == -1)
			ft_exit(&data, "ERROR: Fork", 1);
		else if (data.pid2 == 0)
			child_two(&data, argv[4], envp);
		waitpid(data.pid1, NULL, 0);
		waitpid(data.pid2, NULL, 0);
	}
	return (0);
}
