/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:28:37 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/05 18:41:26 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdio.h>

typedef struct s_data
{
	pid_t	pid1;
	pid_t	pid2;
	char	**cmd;
	char	**paths;
	char	*path;
	int		pipe[2];
	int		in;
	int		out;
}	t_data;

void	child_one(t_data *data, char *command, char **envp);
void	child_two(t_data *data, char *command, char **envp);
void	close_fds(t_data *data);
void	ft_exit(t_data *data, char *msg, int exitcode);
void	get_cmd_path(t_data *data, char *command, char **envp);

#endif
