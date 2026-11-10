#include "minishell.h"

void	redirection_parser(t_token **redir, t_token **tokens)
{
	t_token	*temp;
	t_token	*new;

	(*tokens)->next->type = (*tokens)->type;
	rm_quotes((*tokens)->next, (*tokens)->next->value);
	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new->type = (*tokens)->next->type;
	new->next = NULL;
	new->value = ft_strdup((*tokens)->next->value);
	if (!(new->value))
		return ;
	if (!(*redir))
		*redir = new;
	else
	{
		temp = *redir;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
	*tokens = (*tokens)->next->next;
}

void	arg_cmd(t_list **cmd_arg, t_token **tokens)
{
	char	*word;
	t_list	*node;

	rm_quotes(*tokens, (*tokens)->value);
	if ((*tokens)->value[0] == '\0')
	{
		*tokens = (*tokens)->next;
		return ;
	}
	word = ft_strdup((*tokens)->value);
	if (!word)
		return ;
	node = ft_lstnew(word);
	if (!node)
	{
		free(word);
		return ;
	}
	ft_lstadd_back(cmd_arg, node);
	*tokens = (*tokens)->next;
}

t_list	*command_parsing(t_token **tokens)
{
	t_command	*new_cmd;
	t_list		*cmd_arg;

	cmd_arg = NULL;
	new_cmd = ft_calloc(1, sizeof(t_command));
	if (!new_cmd)
		return (NULL);
	while (*tokens && (*tokens)->type != PIPE)
	{
		expander_dollar(&(*tokens)->value);
		if ((*tokens)->value[0] == '\0')
		{
			*tokens = (*tokens)->next;
			continue ;
		}
		if ((*tokens)->type >= IN && (*tokens)->type <= APPEND)
			redirection_parser(&new_cmd->redir, tokens);
		else if ((*tokens)->type == WORD)
			arg_cmd(&cmd_arg, tokens);
	}
	cmd_array(cmd_arg, new_cmd);
	free_list_cmd(&cmd_arg);
	return (ft_lstnew(new_cmd));
}

t_list	*build_cmd_list(t_token *tokens)
{
	t_list	*lst;
	t_list	*cmd;

	lst = NULL;
	while (tokens)
	{
		if (!tokens->value[0])
			tokens = tokens->next;
		else
		{
			cmd = command_parsing(&tokens);
			if (!cmd)
				return (NULL);
			ft_lstadd_back(&lst, cmd);
			if (tokens && tokens->type == PIPE)
				tokens = tokens->next;
		}
	}
	return (lst);
}

void	parser(t_token *tokens)
{
	t_data	*data;

	if (!tokens)
		return ;
	data = get_data_struct();
	data->cmds = build_cmd_list(tokens);
	free_tokens(&tokens);
}
