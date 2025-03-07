/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 18:02:48 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/07 18:02:53 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	data->path = NULL;
	return (0);
}

int	cmd_found(t_data *data)
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
			ft_error(data, "permission denied", 126);
	}
	return (0);
}
