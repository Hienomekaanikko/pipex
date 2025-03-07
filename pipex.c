/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:42:49 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/07 18:05:38 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	prep_env(t_data *data, int argc, char **argv)
{
	data->in = open(argv[1], O_RDONLY);
	if (data->in < 0)
		perror("infile");
	data->out = open(argv[argc - 1], O_TRUNC | O_CREAT | O_RDWR, 0000644);
	if (data->out < 0)
		perror("outfile");
	if (pipe(data->pipe) < 0)
	{
		if (data->in >= 0)
			close(data->in);
		close(data->out);
		ft_error(data, "pipe", 1);
	}
}

static void	check_args(t_data *data, int argc)
{
	if (argc != 5)
	{
		ft_putendl_fd("Usage: ./pipex infile cmd1 cmd2 outfile", 2);
		ft_error(data, NULL, 1);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	int		status;

	init_data(&data);
	check_args(&data, argc);
	prep_env(&data, argc, argv);
	data.pid1 = fork();
	if (data.pid1 == 0)
		child_one(&data, argv[2], envp);
	else if (data.pid1 == -1)
		ft_error(&data, "pid1 fork fail", 1);
	data.pid2 = fork();
	if (data.pid2 == 0)
		child_two(&data, argv[3], envp);
	else if (data.pid2 == -1)
		ft_error(&data, "pid2 fork fail", 1);
	close_fds(&data);
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			return (WEXITSTATUS(status));
	}
	return (0);
}
