#include "libft.h"

int	ft_error(char *msg)
{
	ft_putendl_fd(msg, 2);
	return (1);
}
