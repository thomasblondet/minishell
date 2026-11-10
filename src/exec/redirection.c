#include "minishell.h"

extern int	g_signal;

bool	manage_redir_in(t_command *command, t_token *src)
{
	if (src->type == HEREDOC || src->type == HEREDOC_WEXP)
	{
		generate_filename((unsigned long)command, command);
		command->stdin = get_heredoc(src, command->filename);
		unlink(command->filename);
	}
	else if (src->type == IN)
		command->stdin = open(src->value, O_RDONLY, 0644);
	if (command->stdin == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(src->value);
		return (false);
	}
	return (true);
}

int	get_heredoc(t_token *src, char *filename)
{
	int		fd_tmp;
	pid_t	pid;

	fd_tmp = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	get_data_struct()->fd_heredoc = fd_tmp;
	signal(SIGINT, &modify_heredoc_signal);
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("unable to fork\n", 2);
		return (-1);
	}
	if (pid == 0)
	{
		signal(SIGINT, &heredoc_signal);
		signal(SIGQUIT, SIG_IGN);
		receive_heredoc(src->type, src->value, fd_tmp);
		free_child_and_exit(0);
	}
	waitpid(pid, NULL, 0);
	signal(SIGINT, &sigint_handler);
	close(fd_tmp);
	return (open(filename, O_RDONLY, 0644));
}

void	receive_heredoc(t_type type, char *delimiter, int fd)
{
	char	*line;

	line = readline("> ");
	if (!line)
		return ;
	if (type == HEREDOC_WEXP)
		expander_dollar(&line);
	while (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) != 0)
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline("> ");
		if (!line)
			return ;
		if (type == HEREDOC_WEXP)
			expander_dollar(&line);
	}
	if (line)
		free(line);
}

void	generate_filename(unsigned long p, t_command *cmd)
{
	char	addrss[17];
	char	*ch_hex;
	int		i;

	ch_hex = "0123456789abcdef";
	i = 0;
	while (i < 16)
	{
		addrss[i] = ch_hex[(p >> (4 * (15 - i))) & 0xf];
		i++;
	}
	addrss[i] = '\0';
	cmd->filename = ft_strjoin(addrss, ".heredoc.tmp");
}

bool	manage_redir_out(t_command *command, t_token *dest)
{
	if (dest->type == APPEND)
		command->stdout = open(dest->value, O_CREAT | O_WRONLY | O_APPEND,
				0644);
	else if (dest->type == OUT)
		command->stdout = open(dest->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (command->stdout == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(dest->value);
		return (false);
	}
	return (true);
}
