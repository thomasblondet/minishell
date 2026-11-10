#include "minishell.h"

int	is_nbr(char *s)
{
	int	i;

	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

int	valid_input(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	if (i == 2 && !is_nbr(args[1]))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (0);
	}
	return (1);
}

void	free_cmd_list(void)
{
	t_data	*data;
	t_list	*next;
	t_list	**commmand_list;

	data = get_data_struct();
	commmand_list = &data->cmds;
	while (*commmand_list)
	{
		next = (*commmand_list)->next;
		free_cmd(&((t_command *)(*commmand_list)->content)->cmd);
		free(((t_command *)(*commmand_list)->content)->fullpath);
		free(((t_command *)(*commmand_list)->content)->filename);
		free_tokens(&((t_command *)(*commmand_list)->content)->redir);
		free((*commmand_list)->content);
		free(*commmand_list);
		*commmand_list = next;
	}
}

int	builtin_exit(char **args)
{
	int	st;
	int	i;

	i = 0;
	st = 0;
	while (args[i])
		i++;
	if ((args[1] && is_nbr(args[1])) && i > 2)
		return (ft_putstr_fd("exit\nminishell: exit: too many arguments\n", 2),
			1);
	printf("exit\n");
	if (i == 2 && valid_input(args))
		st = ft_atoi(args[1]);
	else if (args[1])
	{
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
		st = 2;
	}
	free_cmd_list();
	free_env();
	rl_clear_history();
	exit(st);
}
