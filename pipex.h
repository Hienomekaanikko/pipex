/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:28:37 by msuokas           #+#    #+#             */
/*   Updated: 2025/02/24 15:06:11 by msuokas          ###   ########.fr       */
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
	char	*cmd;
	char	**paths;
	char	*path;
	int		pipe[2];
	int		in;
	int		out;
} t_data;

void	prep_env(t_data *data, int argc, char **argv);
void	get_path(t_data *data, char *cmd, char **envp);
void	child_one(t_data *data, char **argv, char **envp);
void	child_two(t_data *data, char **argv, char **envp);
void	free_data(t_data *data);

#endif
