/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:42:49 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/05 14:15:42 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	init_data(t_data *data)
{
	data->status1 = 0;
	data->status2 = 0;
	data->paths = NULL;
	data->path = NULL;
	data->cmd = NULL;
	data->in = -1;
	data->out = -1;
	data->pipe[0] = -1;
	data->pipe[1] = -1;
}

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
		ft_exit(data, "pipe", 1);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	init_data(&data);
	if (argc != 5)
		ft_exit(&data, "Usage: ./pipex infile cmd1 cmd2 outfile", 1);
	prep_env(&data, argc, argv);
	data.pid1 = fork();
	if (data.pid1 == 0)
		child_one(&data, argv[2], envp);
	else if (data.pid1 == -1)
		ft_exit(&data, "pid1 fork fail", 1);
	data.pid2 = fork();
	if (data.pid2 == 0)
		child_two(&data, argv[3], envp);
	else if (data.pid2 == -1)
		ft_exit(&data, "pid2 fork fail", 1);
	close_fds(&data);
	waitpid(data.pid1, &data.status1, 0);
	waitpid(data.pid2, &data.status2, 0);
	if (WIFEXITED(data.status2))
		return (WEXITSTATUS(data.status2));
	else if (WIFEXITED(data.status1))
		return (WEXITSTATUS(data.status1));
	return (0);
}
