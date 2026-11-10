#include "minishell.h"

int	execute_single_builtin(t_command *command)
{
	t_data	*data;
	char	*cmd_name;
	int		exit_c;

	exit_c = 0;
	data = get_data_struct();
	cmd_name = command->cmd[0];
	if (!ft_strncmp(cmd_name, "echo", 5))
		exit_c = builtin_echo(command->cmd);
	else if (!ft_strncmp(cmd_name, "cd", 3))
		exit_c = builtin_cd(command->cmd, data->env);
	else if (!ft_strncmp(cmd_name, "pwd", 4))
		exit_c = builtin_pwd();
	else if (!ft_strncmp(cmd_name, "export", 7))
		exit_c = builtin_export(command->cmd, data->env);
	else if (!ft_strncmp(cmd_name, "unset", 6))
		exit_c = builtin_unset(command->cmd, data->env);
	else if (!ft_strncmp(cmd_name, "env", 4))
		exit_c = builtin_env(data->env, command->cmd);
	else if (!ft_strncmp(cmd_name, "exit", 5))
		exit_c = builtin_exit(command->cmd);
	return (exit_c);
}

int	builtins_execution(t_command *child)
{
	int	orig;
	int	exit_c;

	exit_c = 0;
	orig = 0;
	if (child->stdout != STDOUT_FILENO)
	{
		orig = dup(STDOUT_FILENO);
		dup2(child->stdout, STDOUT_FILENO);
	}
	exit_c = execute_single_builtin(child);
	if (child->stdout != STDOUT_FILENO)
		dup2(orig, STDOUT_FILENO);
	return (exit_c);
}

void	execute_child(t_command *child_ps)
{
	int	exit_c;

	if (child_ps->stdin == -1 || child_ps->stdout == -1)
		free_child_and_exit(1);
	if (!child_ps->cmd || !child_ps->cmd[0] || child_ps->cmd[0][0] == '\0')
		free_child_and_exit(0);
	if (builtin(child_ps->cmd[0]))
	{
		exit_c = execute_single_builtin(child_ps);
		free_child_and_exit(exit_c);
	}
	if (ft_strchr("./", child_ps->cmd[0][0]) && !valid_path(child_ps, &exit_c))
		free_child_and_exit(exit_c);
	if (!child_ps->fullpath)
	{
		ft_putstr_fd(child_ps->cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		free_child_and_exit(127);
	}
	char **(env) = env_to_array();
	execve(child_ps->fullpath, child_ps->cmd, env);
	perror("minishell");
	close(child_ps->stdin);
	close(child_ps->stdout);
	free_child_and_exit(1);
}

bool	valid_path(t_command *child, int *exit_code)
{
	if (access(child->cmd[0], F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(child->cmd[0]);
		*exit_code = 127;
		return (0);
	}
	if (access(child->cmd[0], X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(child->cmd[0]);
		*exit_code = 126;
		return (0);
	}
	if (is_directory(child->cmd[0]))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(child->cmd[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		*exit_code = 126;
		return (0);
	}
	child->fullpath = child->cmd[0];
	return (1);
}

int	is_directory(char *path)
{
	struct stat	file_stat;

	if (stat(path, &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode))
			return (1);
		else
			return (0);
	}
	return (0);
}
