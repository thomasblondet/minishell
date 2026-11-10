#include "minishell.h"

void	count_quotes(char c, int *squotes, int *dquotes)
{
	if (c == '\"')
	{
		(*dquotes)++;
	}
	else if (c == '\'')
	{
		(*squotes)++;
	}
}

const char	*skip_whitespace(const char *input)
{
	while (*input && ft_isspace(*input))
		input++;
	return (input);
}

int	is_invalid_redirection(const char **input)
{
	const char	*operator_start = *input;

	(*input)++;
	if (*operator_start == **input)
		(*input)++;
	*input = skip_whitespace(*input);
	if (**input == '\0' || ft_strchr("><|", **input))
		return (1);
	return (0);
}

char	*input_trime(char *input)
{
	char	*trim;
	int		start;
	int		end;

	if (!input)
		return (NULL);
	if (ft_strlen(input) == 0)
		return (ft_strdup(input));
	start = 0;
	end = ft_strlen(input) - 1;
	while (input[start] && ft_isspace(input[start]))
		start++;
	while (end && ft_isspace(input[end]))
		end--;
	if (start > end)
		trim = malloc(sizeof(char));
	else
		trim = malloc((end - start + 2) * sizeof(char));
	if (!trim)
		return (NULL);
	if (start > end)
		trim[0] = '\0';
	else
		ft_strlcpy(trim, &input[start], end - start + 2);
	return (trim);
}

int	valid_quotes(const char *input)
{
	char	quote;

	quote = 0;
	while (*input)
	{
		if (ft_strchr("\'\"", *input))
		{
			if (quote == *input)
				quote = 0;
			else if (quote == 0)
				quote = *input;
		}
		input++;
	}
	return (quote != 0);
}
