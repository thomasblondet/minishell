#include "minishell.h"

void	expander_dollar(char **p_str)
{
	int	ipos;

	ipos = 0;
	while (valid_expandable(*p_str, &ipos))
	{
		expander(p_str, ipos);
	}
}

void	expander(char **p_old_str, int head)
{
	char	*new;
	char	*old;
	int		end;
	char	*envvar;

	old = *p_old_str;
	end = head + 1;
	if (old[end] != '?')
	{
		while (ft_isalnum(old[end + 1]) || old[end + 1] == '_')
		{
			end++;
		}
	}
	envvar = search_envvar(&old[head + 1], end - head);
	new = new_string(old, envvar, head, end);
	free(old);
	*p_old_str = new;
}

char	*search_envvar(char *name, int len)
{
	int		last;
	t_list	*env;

	if (len == 1 && !ft_strncmp(name, "?", 1))
	{
		last = get_data_struct()->last_exit;
		if (last == 1)
			return ("1");
		if (last == 2)
			return ("2");
		if (last == 126)
			return ("126");
		if (last == 127)
			return ("127");
		if (last == 130)
			return ("130");
		return ("0");
	}
	env = get_data_struct()->env;
	while (env && ft_strncmp(((t_env *)env->content)->name, name, len))
		env = env->next;
	if (!env)
		return ("");
	else
		return (((t_env *)env->content)->value);
}

void	rm_quotes(t_token *token, char *old_str)
{
	int		i;
	int		pair_of_quotes;
	char	quote;
	char	*new_str;

	i = -1;
	pair_of_quotes = 0;
	while (old_str[++i])
	{
		if (old_str[i] == '\'' || old_str[i] == '\"')
		{
			quote = old_str[i];
			i += 1;
			while (old_str[i] != quote)
				i++;
			if (old_str[i] == quote)
				pair_of_quotes += 1;
		}
	}
	if (!pair_of_quotes && token->type == HEREDOC)
		token->type = HEREDOC_WEXP;
	new_str = new_string2(old_str, pair_of_quotes);
	free(old_str);
	token->value = new_str;
}

void	expand_fullpath(t_list *command_list)
{
	char		**all_path;
	t_command	*command;

	all_path = get_path();
	if (!all_path)
		return ;
	while (command_list)
	{
		command = (t_command *)command_list->content;
		if (command->cmd && !builtin(command->cmd[0])
			&& command->cmd[0][0] != '.' && command->cmd[0][0] != '/')
			join_path(command, command->cmd[0], all_path);
		command_list = command_list->next;
	}
	free_cmd(&all_path);
}
