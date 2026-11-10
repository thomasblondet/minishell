#include "minishell.h"

void	manage_parent_process(int *fd, int *output)
{
	close(fd[1]);
	if (*output != STDIN_FILENO)
		close(*output);
	*output = fd[0];
}

void	child_process(int index, int ncmd, t_command *cmd, int *fd_output)
{
	t_data	*data;
	int		fd[2];

	data = get_data_struct();
	manage_pipe(fd, data, index);
	cmd->i = &index;
	if (data->pids[index] == 0)
	{
		manage_child_input(cmd, fd, fd_output);
		manage_child_output(ncmd, cmd, fd, fd_output);
	}
	else
		manage_parent_process(fd, fd_output);
}
