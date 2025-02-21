/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 10:48:55 by msuokas           #+#    #+#             */
/*   Updated: 2025/02/17 18:15:38 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char *argv[], char *envp)
{
	t_data	data;

	if (argc != 5)
		return (ft_error("Invalid amount of arguments."));
	data.in = open(argv[1], O_RDONLY);
	if (data.in < 0)
		msg_error("Infile");
	data.out = open(argv[argc - 1], O_TRUNC | O_CREAT | O_RDWR, 0000644);
	if (data.out < 0)
		msg_error("Outfile");
	if (pipe(data.pipe) < 0)
		msg_error("Pipe");
	data.paths = find_path(envp);
	data.cmd_paths = ft_split(data.paths, ':');
	data.pid1 = fork();
	if (data.pid1 == 0)
		first_child(data, argv, envp);
	data.pid2 = fork();
	if (data.pid2 == 0)
		second_child(data, argv, envp);
	close_pipes(&data);
	waitpid(data.pid1, NULL, 0);
	waitpid(data.pid2, NULL, 0);
	parent_free(&data);
	return (0);
}
