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

void	check_path(t_data *data, char *cmd, int i)
{
	char	*path;

	path = ft_strjoin(data->paths[i], "/");
	if (!path)
		ft_exit(data, "malloc fail find_path", 1);
	data->path = ft_strjoin(path, cmd);
	free(path);
	path = NULL;
	if (!data->path)
		ft_exit(data, "malloc fail find_path", 1);
}

void	find_path(t_data *data, char *cmd, char **envp)
{
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
		check_path(data, cmd, i);
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

int wait_for_child(t_data *data, int pid, char *error_msg)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
		ft_exit(data, "waitpid fail", 1);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		ft_exit(data, error_msg, WEXITSTATUS(status));
	return (status);
}

int	main(int argc, char **argv, char **envp)
{
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
	wait_for_child(&data, data.pid1, "child_one exited with error");
	wait_for_child(&data, data.pid2, "child_two exited with error");
	ft_exit(&data, NULL, 0);
	return (0);
}
