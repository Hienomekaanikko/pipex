/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:25:43 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/05 19:05:05 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	add_to_path(t_data *data, char *cmd, int i)
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
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		ft_exit(data, "path not found", 126);
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

void	get_cmd_path(t_data *data, char *cmd, char **envp)
{
	int	i;

	if (cmd[0] == '/')
	{
		add_absolute_path(data, cmd);
		if (access(data->path, F_OK) != 0)
			ft_exit(data, "No such file or directory", 127);
		if (access(data->path, X_OK) != 0)
			ft_exit(data, "permission denied", 126);
		return;
	}
	split_directories(data, envp);
	i = 0;
	while (data->paths[i])
	{
		add_to_path(data, cmd, i);
		if (access(data->path, F_OK) == 0)
		{
			if (access(data->path, X_OK) == 0)
			{
				ft_free_split(data->paths);
				data->paths = NULL;
				return;
			}
			else
				ft_exit(data, "permission denied", 126);
		}
		i++;
		free(data->path);
		data->path = NULL;
	}
	ft_exit(data, "command not found", 127);
}

