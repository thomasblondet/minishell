#include "minishell.h"

void	inquote(char c, int *is_inquote)
{
	if (*is_inquote == 0 && c == '\'')
	{
		*is_inquote = 1;
	}
	else if (*is_inquote == 0 && c == '\"')
	{
		*is_inquote = 2;
	}
	else if (*is_inquote == 1 && c == '\'')
	{
		*is_inquote = 0;
	}
	else if (*is_inquote == 2 && c == '\"')
	{
		*is_inquote = 0;
	}
}

t_type	get_type(char c)
{
	if (c == '|')
		return (PIPE);
	else if (c == '<')
		return (IN);
	else if (c == '>')
		return (OUT);
	else
		return (WORD);
}

bool	type_diff(char c1, char c2)
{
	if (get_type(c1) != get_type(c2))
		return (1);
	return (0);
}

void	free_tokens(t_token **tokens)
{
	t_token	*temp;

	temp = NULL;
	while (*tokens)
	{
		temp = (*tokens)->next;
		free((*tokens)->value);
		free(*tokens);
		*tokens = temp;
	}
}
