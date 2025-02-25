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

int	main(int argc, char **argv, char **envp)
{
	t_data  data;

	if (argc != 5)
	{
		fprintf(stderr, "Usage: %s infile cmd1 cmd2 outfile\n", argv[0]);
		return (1);
	}
	prep_env(&data, argc, argv);
	data.pid1 = fork();
	if (data.pid1 < 0)
	{
		perror("Fork failed");
		free_data(&data);
		exit(1);
	}
	else if (data.pid1 == 0)
		child_one(&data, argv, envp);
	data.pid2 = fork();
	if (data.pid2 < 0)
	{
		perror("Fork failed");
		free_data(&data);
		exit(1);
	}
	else if (data.pid2 == 0)
		child_two(&data, argv, envp);
	close(data.pipe[0]);
	close(data.pipe[1]);
	waitpid(data.pid1, NULL, 0);
	waitpid(data.pid2, NULL, 0);
	free_data(&data);
	return (0);
}

