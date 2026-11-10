#include "minishell.h"

void	free_cmd(char ***ptr_to_arr)
{
	char	**start;

	start = *ptr_to_arr;
	if (!start)
		return ;
	while (*start)
	{
		free(*start);
		start++;
	}
	free(*ptr_to_arr);
}

void	free_list_cmd(t_list **cmd)
{
	t_list	*next;

	while (*cmd)
	{
		next = (*cmd)->next;
		free((*cmd)->content);
		free(*cmd);
		*cmd = next;
	}
}

void	cmd_array(t_list *cmd_arg, t_command *cmd)
{
	char	**ptr;
	int		wcount;

	wcount = ft_lstsize(cmd_arg);
	if (!wcount)
		return ;
	cmd->cmd = malloc(sizeof(char *) * (wcount + 1));
	if (!(cmd->cmd))
		return ;
	ptr = cmd->cmd;
	while (cmd_arg)
	{
		*ptr = ft_strdup((char *)(cmd_arg->content));
		if (!(*ptr))
			free_cmd(&cmd->cmd);
		cmd_arg = cmd_arg->next;
		ptr++;
	}
	*ptr = NULL;
}
