#include "minishell.h"

extern int	g_signal;

void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	get_data_struct()->last_exit = 130;
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
}

void	sigint_child(int sig)
{
	g_signal = sig;
	if (sig == 2)
		get_data_struct()->last_exit = 130;
	else
	{
		ft_putstr_fd("quit", 1);
		get_data_struct()->last_exit = 131;
	}
	ft_putstr_fd("\n", 1);
}

void	init_signal(void)
{
	g_signal = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	modify_heredoc_signal(int sig)
{
	g_signal = sig;
	get_data_struct()->last_exit = 130;
}

void	heredoc_signal(int sig)
{
	g_signal = sig;
	printf("\n");
	heredoc_free_exit(130);
}
