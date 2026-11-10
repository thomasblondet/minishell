#include "minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	nflag;

	nflag = 0;
	i = 1;
	while (args[i] && ft_strncmp("-n", args[i], 2) == 0)
	{
		nflag = 1;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!nflag)
		printf("\n");
	return (0);
}
