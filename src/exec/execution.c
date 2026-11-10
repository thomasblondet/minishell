#include "minishell.h"

void	manage_redir(t_list *command)
{
	t_command	*current;
	t_token		*token;
	int			valid_redirection;

	while (command)
	{
		valid_redirection = 1;
		current = (t_command *)command->content;
		token = current->redir;
		while (token && valid_redirection)
		{
			if (token->type == HEREDOC || token->type == HEREDOC_WEXP
				|| token->type == IN)
				valid_redirection = manage_redir_in(current, token);
			else
				valid_redirection = manage_redir_out(current, token);
			token = token->next;
		}
		if (current->stdout == 0)
			current->stdout = STDOUT_FILENO;
		command = command->next;
	}
}

void	execute_single_cmd(t_command *command)
{
	int		status;
	pid_t	pid;

	if (command->stdin == -1 || command->stdout == -1)
		get_data_struct()->last_exit = 1;
	if (command->stdin == -1 || command->stdout == -1 || !command->cmd)
		return ;
	if (builtin(command->cmd[0]))
	{
		get_data_struct()->last_exit = builtins_execution(command);
		return ;
	}
	pid = fork();
	signal(SIGINT, sigint_child);
	signal(SIGQUIT, sigint_child);
	if (!pid)
	{
		dup2(command->stdin, STDIN_FILENO);
		dup2(command->stdout, STDOUT_FILENO);
		execute_child(command);
	}
	waitpid(pid, &status, 0);
	actualize_exit_status(status);
	init_signal();
}

void	executor(void)
{
	int		ncmd;
	t_data	*data;
	t_list	*command;

	data = get_data_struct();
	command = data->cmds;
	ncmd = ft_lstsize(command);
	if (!ncmd)
		return ;
	if (ncmd > 1024)
	{
		ft_putstr_fd("Pipe not working\n", 2);
		data->last_exit = 1;
		return ;
	}
	manage_redir(command);
	if (are_cmds_failed(command))
		data->last_exit = 1;
	expand_fullpath(command);
	if (ncmd == 1)
		execute_single_cmd(command->content);
	else
		pipex(data, command, ncmd);
}

void	actualize_exit_status(int st)
{
	if (WIFEXITED(st) && get_data_struct()->last_exit == 0)
		get_data_struct()->last_exit = WEXITSTATUS(st);
	else if (WIFSIGNALED(st) && get_data_struct()->last_exit == 0)
		get_data_struct()->last_exit = WTERMSIG(st);
}

bool	builtin(char *cmd_name)
{
	if (!ft_strncmp(cmd_name, "cd", 3))
		return (true);
	if (!ft_strncmp(cmd_name, "pwd", 4))
		return (true);
	if (!ft_strncmp(cmd_name, "echo", 5))
		return (true);
	if (!ft_strncmp(cmd_name, "unset", 6))
		return (true);
	if (!ft_strncmp(cmd_name, "exit", 5))
		return (true);
	if (!ft_strncmp(cmd_name, "export", 7))
		return (true);
	if (!ft_strncmp(cmd_name, "env", 4))
		return (true);
	return (false);
}
