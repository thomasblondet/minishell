#include "minishell.h"

int	builtin_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		return (ft_putstr_fd("pwd: error current directory\n", 2), 1);
	printf("%s\n", path);
	free(path);
	return (0);
}
