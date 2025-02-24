/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:29:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/02/24 15:26:20 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_path(char *envp)
{
	while (ft_strncmp("PATH", envp, 4))
		envp++;
	return (envp + 5);
}

static void	child_one(t_data data, char **argv, char **envp)
{
	dup2(data.pipe[1], 1);
	dup2(data.in, 0);
	data.cmds = ft_split(argv[2], ' ');
	if (!data.cmds)
		return ;
	data.cmd = get_command(data.cmds, data.cmd);

}

static void	child_two(t_data data, char **argv, char **envp)
{
	dup2(data.pipe[0], 0);
	dup2(data.out, 1);
	data.cmds = ft_split(argv[3], ' ');
	if (!data.cmds)
		return ;
	data.cmd = get_command(data.cmds, data.cmd);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	if (argc != 5)
		return (0);
	data.in = open(argv[0], O_RDONLY);
	if (data.in < 0)
		return (0); //add the correct exit function later
	data.out = open(argv[argc - 1], O_TRUNC | O_CREAT | O_RDWR, 0000644);
	if (data.out < 0)
		return (0); //add the correct exit function later
	if (pipe(data.pipe) < 0)
		return (0); //add the correct exit function later
	data.pid1 = fork();
	if (data.pid1 == 0)
		child_one(data, argv, envp);
	data.pid2 = fork();
	if (data.pid2 == 0)
		child_two(data, argv, envp);
	waitpid(data.pid1, NULL, 0);
	waitpid(data.pid2, NULL, 0);
	//free processes
	exit(1);
}
