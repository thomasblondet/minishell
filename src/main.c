#include "minishell.h"

int		g_signal;

bool	duplicate_env(char **env, t_data *shell)
{
	int		i;
	t_env	*env_var;
	t_list	*env_var_node;

	i = -1;
	shell->env = NULL;
	if (!env)
		return (0);
	while (env[++i])
	{
		env_var = create_env_var(env[i]);
		if (!env_var)
			return (0);
		env_var_node = ft_lstnew(env_var);
		if (!env_var_node)
			return (0);
		ft_lstadd_back(&shell->env, env_var_node);
	}
	return (1);
}

void	free_env(void)
{
	t_list	*tmp;
	t_list	*env;

	env = get_data_struct()->env;
	while (env)
	{
		tmp = env;
		env = env->next;
		free(((t_env *)tmp->content)->name);
		free(((t_env *)tmp->content)->value);
		free(tmp->content);
		free(tmp);
	}
}

void	loop_sh(void)
{
	t_token	*tokens;
	char	*input;

	while (1)
	{
		input = readline("$   ");
		if (!input)
			break ;
		if (is_empty(input))
			continue ;
		add_history(input);
		tokens = lexer(input);
		if (!tokens)
			continue ;
		parser(tokens);
		executor();
		free_cmd_list();
	}
	rl_clear_history();
}

t_data	*get_data_struct(void)
{
	static t_data	data;

	return (&data);
}

int	main(int argc, char **argv, char **env)
{
	t_data	*data;

	(void)argc;
	(void)argv;
	data = get_data_struct();
	g_signal = 0;
	if (!env)
		return (EXIT_FAILURE);
	if (duplicate_env(env, data) == 0)
		return (EXIT_FAILURE);
	init_signal();
	loop_sh();
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", 2);
	free_env();
	return (EXIT_SUCCESS);
}
