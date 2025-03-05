/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:22:32 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/05 19:09:27 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	is_space(char *cmd)
{
	int	i;

	i = 0;
	while(cmd[i])
	{
		if (ft_isspace(cmd[i]))
			i++;
		else
			return (0);
	}
	return (1);
}

void	child_one(t_data *data, char *cmd, char **envp)
{
	if (!cmd || is_space(cmd))
	{
		perror("Command not found");
		exit(127);
	}
	data->cmd = ft_split(cmd, ' ');
	if (!data->cmd)
		ft_exit(data, "malloc fail child one", 1);
	get_cmd_path(data, data->cmd[0], envp);
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
	if (!cmd || is_space(cmd))
	{
		perror("Command not found");
		exit(127);
	}
	data->cmd = ft_split(cmd, ' ');
	if (!data->cmd)
		ft_exit(data, "malloc fail child two", 1);
	get_cmd_path(data, data->cmd[0], envp);
	if (dup2(data->out, 1) == -1)
		ft_exit(data, "dup2 fail child two", 1);
	if (dup2(data->pipe[0], 0) == -1)
		ft_exit(data, "dup2 fail child two", 1);
	close_fds(data);
	if (execve(data->path, data->cmd, envp) == -1)
		ft_exit(data, "execve fail child two", 1);
}
