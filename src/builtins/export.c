#include "minishell.h"

int	add_back(char *s, t_list **env)
{
	t_list	*envnode;
	t_env	*envvar;
	int		i;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	envvar = malloc(sizeof(t_env));
	if (!envvar)
		return (1);
	envvar->name = ft_substr(s, 0, i);
	if (!envvar->name)
		return (1);
	envvar->value = ft_strdup(&s[i + 1]);
	if (!envvar->value)
		return (1);
	envnode = ft_lstnew(envvar);
	if (!envvar)
		return (1);
	ft_lstadd_back(env, envnode);
	return (0);
}

bool	name_validity(const char *s)
{
	int	i;

	i = 0;
	if (!s || !(*s) || *s == '=' || ft_isdigit(*s))
		return (false);
	while (s[i] && s[i] != '=')
	{
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

void	modify_var(char *s, t_list **env_list)
{
	int		i;
	t_env	*var;
	t_list	*curr;

	i = 0;
	curr = *env_list;
	while (s[i] && s[i] != '=')
		i++;
	while (curr)
	{
		var = (t_env *)curr->content;
		if (!ft_strncmp(var->name, s, i) && ft_strlen(var->name) == (size_t)i)
		{
			free(var->value);
			var->value = ft_strdup(&s[i + 1]);
			return ;
		}
		curr = curr->next;
	}
	if (!curr)
		add_back(s, env_list);
}

void	print_invalid(char *arg)
{
	char	*final;
	char	*temp;

	temp = ft_strjoin("minishell: export: `", arg);
	final = ft_strjoin(temp, "': not a valid identifier\n");
	ft_putstr_fd(final, 2);
	free(temp);
	free(final);
}

int	builtin_export(char **args, t_list *env)
{
	int	i;
	int	exit_c;

	exit_c = 0;
	if (!args[1])
		builtin_env(env, args);
	i = 0;
	while (args[++i])
	{
		if (!ft_strchr(args[i], '=') && name_validity(args[i]))
			continue ;
		if (!name_validity(args[i]))
		{
			print_invalid(args[i]);
			exit_c = 1;
			continue ;
		}
		modify_var(args[i], &env);
	}
	return (exit_c);
}
