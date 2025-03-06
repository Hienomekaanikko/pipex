/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:25:43 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/06 17:52:09 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	add_cmd_to_path(t_data *data, char *cmd, int i)
{
	char	*path;

	path = ft_strjoin(data->paths[i], "/");
	if (!path)
		ft_exit(data, "malloc fail add_to_path", 1);
	data->path = ft_strjoin(path, cmd);
	free(path);
	path = NULL;
	if (!data->path)
		ft_exit(data, "malloc fail add_to_path", 1);
}

static void	split_directories(t_data *data, char **envp)
{
	int	i;

	i = 0;
	if (!envp[0])
		exit(127);
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		ft_exit(data, "PATH not set", 127);
	data->paths = ft_split(envp[i] + 5, ':');
	if (!data->paths)
		ft_exit(data, "malloc fail split_directories", 1);

}

static void	add_absolute_path(t_data *data, char *cmd)
{
	data->path = ft_strdup(cmd);
	if (!data->path)
		ft_exit(data, "malloc fail add_absolute_path", 1);
}

int	is_relative_path(t_data *data, char *cmd)
{
	if (access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) == 0)
		{
			data->path = cmd;
			return (1);
		}
	}
	return (0);
}

int	is_right_path(t_data *data)
{
	if (access(data->path, F_OK) == 0)
	{
		if (access(data->path, X_OK) == 0)
		{
			ft_free_split(data->paths);
			data->paths = NULL;
			return (1);
		}
		else
			ft_exit(data, "permission denied", 126);
	}
	return (0);
}

void	get_cmd_path(t_data *data, char *cmd, char **envp)
{
	int	i;

	if (is_relative_path(data, cmd))
		return ;
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		add_absolute_path(data, cmd);
		if (access(data->path, F_OK) != 0)
			ft_exit(data, "no such file or directory", 127);
		if (access(data->path, X_OK) != 0)
			ft_exit(data, "permission denied", 126);
		return;
	}
	split_directories(data, envp);
	i = 0;
	while (data->paths[i])
	{
		add_cmd_to_path(data, cmd, i);
		if (is_right_path(data))
			return;
		i++;
		free(data->path);
		data->path = NULL;
	}
	ft_exit(data, "command not found", 127);
}

