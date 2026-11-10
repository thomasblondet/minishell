#include "minishell.h"

bool	is_empty(char *input)
{
	char	*tmp;

	tmp = input;
	while (*tmp && ft_isspace(*tmp))
		tmp++;
	if (*tmp == '\0')
	{
		free(input);
		return (true);
	}
	return (false);
}

void	free_child_and_exit(int exit_code)
{
	free_cmd_list();
	free_env();
	rl_clear_history();
	exit(exit_code);
}

t_env	*create_env_var(char *env)
{
	int		j;
	t_env	*env_var;

	j = 0;
	while (env[j] != '=')
		j++;
	env_var = malloc(sizeof(t_env));
	if (!env_var)
		return (NULL);
	env_var->name = ft_substr(env, 0, j);
	if (!env_var->name)
		return (NULL);
	env_var->value = ft_strdup(&env[j + 1]);
	if (!env_var->value)
		return (NULL);
	return (env_var);
}

void	heredoc_free_exit(int exit_code)
{
	int	fd;

	fd = get_data_struct()->fd_heredoc;
	close(fd);
	free_cmd_list();
	free_env();
	rl_clear_history();
	exit(exit_code);
}
