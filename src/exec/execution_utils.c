#include "minishell.h"

bool	are_cmds_failed(t_list *command_list)
{
	t_command	*curr_cmd;

	while (command_list)
	{
		curr_cmd = (t_command *)command_list->content;
		if (curr_cmd->stdin == -1 || curr_cmd->stdout == -1)
			return (true);
		command_list = command_list->next;
	}
	return (false);
}
