/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:22:32 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/05 14:15:57 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_one(t_data *data, char *cmd, char **envp)
{
	if (!cmd[0])
		ft_exit(data, "empty cmd child one", 1);
	data->cmd = ft_split(cmd, ' ');
	if (!data->cmd)
		ft_exit(data, "malloc fail child one", 1);
	get_path(data, data->cmd[0], envp);
	if (dup2(data->in, 0) == -1)
		ft_exit(data, "dup2 fail child one", 1);
	if (dup2(data->pipe[1], 1) == -1)
		ft_exit(data, "dup2 fail child one", 1);
	close_fds(data);
	if (execve(data->path, data->cmd, envp) == -1)
		ft_exit(data, "execve fail child one", 1);
}

void	child_two(t_data *data, char *cmd, char **envp)
{
	if (!cmd[0])
		ft_exit(data, "empty cmd child two", 1);
	data->cmd = ft_split(cmd, ' ');
	if (!data->cmd)
		ft_exit(data, "malloc fail child two", 1);
	get_path(data, data->cmd[0], envp);
	if (dup2(data->out, 1) == -1)
		ft_exit(data, "dup2 fail child two", 1);
	if (dup2(data->pipe[0], 0) == -1)
		ft_exit(data, "dup2 fail child two", 1);
	close_fds(data);
	if (execve(data->path, data->cmd, envp) == -1)
		ft_exit(data, "execve fail child two", 1);
}
