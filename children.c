/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:22:32 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/07 18:01:49 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	is_space(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
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
		perror("Command not found");
	data->cmd = parse_cmd(cmd);
	if (!data->cmd)
		ft_error(data, "malloc fail child one", 1);
	if (!is_relative_path(data, data->cmd[0]))
		get_cmd_path(data, data->cmd[0], envp);
	if (data->path)
	{
		if (access(data->path, F_OK) != 0)
			perror("no such file or directory");
		if (access(data->path, X_OK) != 0)
			perror("permission denied");
	}
	if (!data->path)
		perror("command not found");
	if (dup2(data->in, 0) == -1)
		perror("dup2 fail child one");
	if (dup2(data->pipe[1], 1) == -1)
		perror("dup2 fail child one");
	close_fds(data);
	if (execve(data->path, data->cmd, envp) == -1)
		ft_error(data, "execve fail child one", 0);
}

void	child_two(t_data *data, char *cmd, char **envp)
{
	if (!cmd || is_space(cmd))
		ft_error(data, "command not found", 127);
	data->cmd = parse_cmd(cmd);
	if (!data->cmd)
		ft_error(data, "malloc fail child two", 1);
	if (!is_relative_path(data, data->cmd[0]))
		get_cmd_path(data, data->cmd[0], envp);
	if (data->out < 0)
		ft_error(data, "no such file or directory", 1);
	if (access(data->path, F_OK) != 0)
		ft_error(data, "no such file or directory", 127);
	if (access(data->path, X_OK) != 0)
		ft_error(data, "permission denied", 126);
	if (!data->path)
		perror("command not found");
	if (dup2(data->out, 1) == -1)
		ft_error(data, "dup2 fail child two", 1);
	if (dup2(data->pipe[0], 0) == -1)
		ft_error(data, "dup2 fail child two", 1);
	close_fds(data);
	if (execve(data->path, data->cmd, envp) == -1)
		ft_error(data, "execve fail child two", 0);
}
