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
	t_data	data;

	if (argc == 5)
	{
		prep_env(&data, argc, argv);
		data.pid1 = fork();
		init_child_one(&data, argv, envp);
		data.pid2 = fork();
		init_child_two(&data, argv, envp);
		close(data.pipe[0]);
		close(data.pipe[1]);
		waitpid(data.pid1, NULL, 0);
		waitpid(data.pid2, NULL, 0);
		close(data.in);
		close(data.out);
	}
	return (0);
}
