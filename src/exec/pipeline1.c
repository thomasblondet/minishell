#include "minishell.h"

void	pipex(t_data *data, t_list *lst_cmd, int ncmd)
{
	int			st;
	t_command	*current;
	int			i;
	int			output;

	(void)data;
	i = 1;
	output = STDIN_FILENO;
	while (lst_cmd)
	{
		current = (t_command *)lst_cmd->content;
		child_process(i, ncmd, current, &output);
		lst_cmd = lst_cmd->next;
		i++;
	}
	i = 0;
	while (i < ncmd)
	{
		waitpid(data->pipe[i].pid, &st, 0);
		i++;
	}
	if (WIFEXITED(st))
		get_data_struct()->last_exit = WEXITSTATUS(st);
	else if (WIFSIGNALED(st))
		get_data_struct()->last_exit = WTERMSIG(st);
}

void	manage_pipe(int *fd, t_data *data, int i)
{
	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	data->pids[i] = fork();
	if (data->pids[i] == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
}

void	manage_last_command(t_command *command, int *fd)
{
	if (command->stdout != STDOUT_FILENO)
	{
		if (dup2(command->stdout, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(command->stdout);
	}
	close(fd[1]);
}

void	manage_child_input(t_command *command, int *fd, int *output)
{
	close(fd[0]);
	if (command->stdin != STDIN_FILENO)
	{
		close(*output);
		if (dup2(command->stdin, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(command->stdin);
	}
	else if (*command->i != 1)
	{
		if (dup2(*output, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(*output);
	}
}

void	manage_child_output(int ncmd, t_command *command, int *fd, int *output)
{
	if (command->stdout != STDOUT_FILENO)
	{
		close(*output);
		if (dup2(command->stdout, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(command->stdout);
	}
	else if (*command->i < ncmd)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(fd[1]);
	}
	else
		manage_last_command(command, fd);
	execute_child(command);
	perror("minishell");
	exit(EXIT_FAILURE);
}
