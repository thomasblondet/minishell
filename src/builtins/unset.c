#include "minishell.h"

void	link_and_free(t_list *prev, t_list *current)
{
	t_env	*envvar;

	prev->next = current->next;
	envvar = (t_env *)current->content;
	free(envvar->name);
	free(envvar->value);
	free(current->content);
	free(current);
}

void	unset_var(char *arg, t_list **env)
{
	int		len;
	t_list	*current;
	t_list	*prev;
	t_env	*envvar;

	envvar = NULL;
	prev = NULL;
	current = *env;
	len = ft_strlen(arg);
	while (current)
	{
		envvar = (t_env *)current->content;
		if (ft_strncmp(envvar->name, arg, len + 1) == 0)
		{
			if (prev)
				link_and_free(prev, current);
			else
				*env = current->next;
			break ;
		}
		prev = current;
		current = current->next;
	}
}

int	builtin_unset(char **args, t_list *env)
{
	int	i;

	i = 1;
	if (!args[1])
		return (0);
	while (args[i])
	{
		unset_var(args[i], &env);
		i++;
	}
	return (0);
}
