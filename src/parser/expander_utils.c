#include "minishell.h"

bool	valid_expandable(char *orig, int *idollar)
{
	char	*s;
	int		open_quote;

	s = orig;
	open_quote = 0;
	while (*s)
	{
		if (*s == '\'' || *s == '\"')
		{
			if (open_quote == 0)
				open_quote = *s;
			else if (open_quote == *s)
				open_quote = 0;
		}
		else if (open_quote != '\'' && *s == '$' && ((ft_isalnum(*(s + 1))
					|| *(s + 1) == '_') || *(s + 1) == '?'))
		{
			*idollar = s - orig;
			return (1);
		}
		s++;
	}
	return (0);
}

char	*new_string(char *old, char *value, int start, int end)
{
	int		ltotal;
	char	*new;
	int		lstart;
	int		lvalue;

	lvalue = ft_strlen(value);
	lstart = ft_strlen(old) - (end + 1);
	ltotal = ft_strlen(old) - (end - start + 1) + lvalue;
	new = malloc(ltotal + 1);
	if (!new)
		return (NULL);
	ft_strlcpy(new, old, start + 1);
	ft_strlcpy(new + start, value, lvalue + 1);
	ft_strlcpy(new + start + lvalue, old + end + 1, lstart + 1);
	return (new);
}

char	*new_string2(char *old, int quotes)
{
	char	*new;
	int		lnew;
	char	quote;
	int		i;

	lnew = ft_strlen(old) - 2 * quotes;
	new = malloc(lnew + 1);
	if (!new)
		return (NULL);
	i = 0;
	while (*old)
	{
		if (*old == '\'' || *old == '\"')
		{
			quote = *old;
			while (*(++old) != quote)
				new[i++] = *old;
		}
		else
			new[i++] = *old;
		old++;
	}
	new[i] = '\0';
	return (new);
}

void	join_path(t_command *cmd, char *cmd_name, char **path_array)
{
	int		i;
	char	*tmp;

	i = 0;
	while (!cmd->fullpath && cmd_name && path_array[i])
	{
		tmp = ft_strjoin(path_array[i], "/");
		cmd->fullpath = ft_strjoin(tmp, cmd_name);
		if (access(cmd->fullpath, F_OK) != 0)
		{
			free(cmd->fullpath);
			cmd->fullpath = NULL;
		}
		free(tmp);
		i++;
	}
}

char	**get_path(void)
{
	char	**all_path;
	t_list	*env_lst;

	env_lst = get_data_struct()->env;
	while (env_lst && ft_strncmp(((t_env *)env_lst->content)->name, "PATH", 5))
		env_lst = env_lst->next;
	if (env_lst && !ft_strncmp(((t_env *)env_lst->content)->name, "PATH", 5))
		all_path = ft_split(((t_env *)env_lst->content)->value, ':');
	else
		all_path = NULL;
	return (all_path);
}
