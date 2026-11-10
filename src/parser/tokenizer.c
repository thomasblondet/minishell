#include "minishell.h"

t_token	*new_node(t_type type, char *value)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->type = type;
	new->value = ft_strdup(value);
	if (!new->value)
		return (free(new), NULL);
	new->next = NULL;
	return (new);
}

t_token	*word_token(char *start, char **end)
{
	t_token	*new;
	char	*word;

	new = NULL;
	word = NULL;
	word = malloc((*end - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
	ft_strlcpy(word, start, *end - start + 1);
	new = new_node(WORD, word);
	if (!new)
		return (NULL);
	free(word);
	while (**end && ft_isspace(**end))
		(*end)++;
	return (new);
}

t_token	*meta(char *start, char **end)
{
	t_token	*new;

	new = NULL;
	if (*start == '<' && (*end - start == 1))
		new = new_node(IN, "<");
	if (*start == '<' && (*end - start == 2))
		new = new_node(HEREDOC, "<<");
	if (*start == '>' && (*end - start == 1))
		new = new_node(OUT, ">");
	if (*start == '>' && (*end - start == 2))
		new = new_node(APPEND, ">>");
	if (*start == '|')
		new = new_node(PIPE, "|");
	while (**end && ft_isspace(**end))
		(*end)++;
	return (new);
}

t_token	*cat(t_token *tokens, t_token *new)
{
	t_token	*last;

	if (!new)
		return (free_tokens(&tokens),
			ft_putstr_fd("Error: tokenization: malloc error.\n", 2), NULL);
	if (!tokens)
		tokens = new;
	else
	{
		last = tokens;
		while (last->next)
			last = last->next;
		last->next = new;
	}
	return (tokens);
}

t_token	*tokenizer(char *line)
{
	t_token	*tokens;
	int		is_inquote;
	char	*start;

	is_inquote = 0;
	tokens = NULL;
	while (*line)
	{
		start = line;
		while (*line)
		{
			inquote(*line, &is_inquote);
			line++;
			if (!is_inquote && (ft_isspace(*line) || type_diff(*start, *line)))
				break ;
		}
		if (*start && ft_strchr("<>|", *start))
			tokens = cat(tokens, meta(start, &line));
		else
			tokens = cat(tokens, word_token(start, &line));
		if (!tokens)
			break ;
	}
	return (tokens);
}
