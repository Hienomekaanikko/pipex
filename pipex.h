/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:28:37 by msuokas           #+#    #+#             */
/*   Updated: 2025/02/27 15:58:58 by msuokas          ###   ########.fr       */
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
	char	**cmd1;
	char	**cmd2;
	char	**paths;
	char	*path;
	int		pipe[2];
	int		in;
	int		out;
} t_data;

void	init_data(t_data *data);
void	prep_env(char **argv, t_data *data);
void	close_fds(t_data *data);
void	ft_exit(t_data *data, char *msg, int exitcode);
void	child_one(t_data *data, char *cmd, char **envp);
void	child_two(t_data *data, char *cmd, char **envp);
void	find_path(t_data *data, char *cmd, char **envp);

#endif
