/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:23:44 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/05 13:55:28 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	if (data->paths)
		ft_free_split(data->paths);
	if (data->path)
		free(data->path);
	if (data->cmd)
		ft_free_split(data->cmd);
	if (msg != NULL)
		perror(msg);
	exit(exitcode);
}
