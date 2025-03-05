/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:25:43 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/05 14:16:08 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	check_path(t_data *data, char *cmd, int i)
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

void	get_path(t_data *data, char *cmd, char **envp)
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
			data->paths = NULL;
			return ;
		}
		free(data->path);
		data->path = NULL;
		i++;
	}
	ft_exit(data, "path not found", 127);
}
