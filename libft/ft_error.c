#include "libft.h"

void	ft_error(char *msg, char *argv)
{
	char	*error_msg;

	error_msg = ft_strjoin(msg, argv);
	ft_putendl_fd(error_msg, 2);
	free(error_msg);
	exit(1);
}
