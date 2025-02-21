#include "pipex.h"

char	*find_path(char *envp)
{
	while (ft_strncmp("PATH", *envp, 4))
		envp++;
	return (*envp + 5);
}
