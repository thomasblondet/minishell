#include "minishell.h"

bool	valid_redirections(const char *input)
{
	int	squotes;
	int	dquotes;

	squotes = 0;
	dquotes = 0;
	while (*input)
	{
		(*input, &squotes, &dquotes);
		if (!(squotes % 2) && !(dquotes % 2) && ft_strchr("><", *input))
		{
			if (is_invalid_redirection(&input))
				return (true);
		}
		else
			input++;
	}
	return (false);
}

bool	operator_validity(const char *input)
{
	int	sq;
	int	dq;
	int	next_cmd;

	sq = 0;
	dq = 0;
	next_cmd = 0;
	if (ft_strchr("|&", *input))
		return (1);
	while (*input)
	{
		count_quotes(*input, &sq, &dq);
		if (*input == '|' && !(sq % 2) && !(dq % 2))
		{
			if (next_cmd)
				return (true);
			next_cmd = 1;
		}
		else if (!ft_isspace(*input))
			next_cmd = 0;
		input++;
	}
	if (next_cmd)
		return (true);
	return (false);
}

bool	bonus_operators(const char *input)
{
	int	sq;
	int	dq;

	sq = 0;
	dq = 0;
	while (*input)
	{
		count_quotes(*input, &sq, &dq);
		if (!(dq % 2) && !(sq % 2) && ((*input == '&' && *(input + 1) == '&')
				|| (*input == '|' && *(input + 1) == '|')))
			return (true);
		input++;
	}
	return (false);
}

int	valid_syntax(const char *input)
{
	if (valid_quotes(input))
		return (ft_putstr_fd("Error: unclosed quote\n", STDERR_FILENO), 1);
	if (valid_redirections(input))
		return (ft_putstr_fd("Error: invalid redirection\n", STDERR_FILENO), 1);
	if (operator_validity(input))
		return (ft_putstr_fd("Error: invalid use of operator\n", STDERR_FILENO),
			1);
	if (bonus_operators(input))
		return (ft_putstr_fd("No Bonus.\n", STDERR_FILENO), 1);
	return (0);
}

t_token	*lexer(char *input)
{
	t_token	*tokens;
	char	*trim;

	trim = input_trime(input);
	tokens = NULL;
	free(input);
	if (!trim)
		return (NULL);
	if (valid_syntax(trim))
	{
		get_data_struct()->last_exit = 2;
		free(trim);
		return (NULL);
	}
	tokens = tokenizer(trim);
	free(trim);
	return (tokens);
}
