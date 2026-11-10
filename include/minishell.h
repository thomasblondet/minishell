#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_env
{
	char			*name;
	char			*value;
}					t_env;

typedef enum e_type
{
	WORD,
	PIPE,
	IN,
	HEREDOC,
	HEREDOC_WEXP,
	OUT,
	APPEND,
}					t_type;

typedef struct s_token
{
	t_type			type;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef struct s_command
{
	char			**cmd;
	char			*fullpath;
	char			*filename;
	int				stdin;
	int				stdout;
	int				*i;
	t_token			*redir;
}					t_command;

typedef struct s_pipe
{
	int				fd[2];
	pid_t			pid;
}					t_pipe;

typedef struct s_data
{
	t_list			*env;
	t_list			*cmds;
	t_pipe			pipe[1024];
	pid_t			pids[1024];
	int				last_exit;
	int				fd_heredoc;
}					t_data;

// struct
t_data				*get_data_struct(void);
bool				duplicate_env(char **envp, t_data *shell);
void				free_env(void);
void				loop_sh(void);

// utils
t_env				*create_env_var(char *envp);
char				*get_prompt(void);
char				*get_pwd(void);
bool				is_empty(char *input);
void				free_child_and_exit(int exit_code);

// signal
void				init_signal(void);
void				sigint_handler(int sig);
void				sigint_child(int sig);
void				modify_heredoc_signal(int sig);
void				heredoc_signal(int sig);
void				heredoc_free_exit(int exit_code);

// syntax
t_token				*lexer(char *input);
int					valid_syntax(const char *input);
char				*input_trime(char *input);
int					valid_quotes(const char *input);
bool				valid_redirections(const char *input);
bool				operator_validity(const char *input);
bool				bonus_operators(const char *input);
void				count_quotes(char c, int *single_quote_count,
						int *double_quote_count);
int					is_invalid_redirection(const char **input);
const char			*skip_whitespace(const char *input);

// token
t_token				*tokenizer(char *line);
t_token				*cat(t_token *tokens, t_token *new);
t_token				*meta(char *start, char **end);
t_token				*word_token(char *start, char **end);
t_token				*new_node(t_type type, char *value);
bool				type_diff(char c1, char c2);
void				inquote(char c, int *in_quote);
void				free_tokens(t_token **tokens);

// parser
void				parser(t_token *tokens);
t_list				*build_cmd_list(t_token *tokens);
t_list				*command_parsing(t_token **tokens);
void				redirection_parser(t_token **redir_list, t_token **tokens);
void				arg_cmd(t_list **cmd_arg, t_token **tokens);
void				cmd_array(t_list *cmd_arg, t_command *cmd);
void				free_list_cmd(t_list **cmd_arg);
void				free_cmd(char ***p_str_arr);

// expander
void				expander_dollar(char **str);
bool				valid_expandable(char *original, int *dollar_pos);
void				expander(char **p_old_str, int head);
char				*search_envvar(char *name, int len);
char				*new_string(char *old_str, char *value, int head, int end);
char				*new_string2(char *old_str, int pair_of_quotes);
void				rm_quotes(t_token *token, char *old_str);

// redirection
void				manage_redir(t_list *cmds);
bool				manage_redir_in(t_command *cmd, t_token *src);
void				generate_filename(unsigned long p, t_command *cmd);
int					get_heredoc(t_token *src, char *filename);
void				receive_heredoc(t_type type, char *delimiter, int fd);
bool				manage_redir_out(t_command *cmd, t_token *dst);

// path
void				expand_fullpath(t_list *cmds);
char				**get_path(void);
void				join_path(t_command *cmd, char *cmd_name, char **paths);
bool				valid_path(t_command *child, int *exit_code);
int					is_directory(char *path);

// execution
void				executor(void);
void				execute_child(t_command *child);
char				**env_to_array(void);
char				**create_env_array(t_list *env);
void				gen_env_dict(char **env_array, t_list *env);
void				execute_single_cmd(t_command *command);
bool				builtin(char *cmd_name);
bool				are_cmds_failed(t_list *command_list);
int					builtins_execution(t_command *child);
int					execute_single_builtin(t_command *cmd);
void				actualize_exit_status(int status);
void				pipex(t_data *data, t_list *cmds, int nb_cmds);
void				child_process(int i, int nb_cmds, t_command *cmd,
						int *output_cache);
void				manage_pipe(int *fd, t_data *data, int i);
void				manage_parent_process(int *fd, int *output_fd);
void				manage_child_input(t_command *command, int *fd,
						int *output);
void				manage_child_output(int ncmd, t_command *command, int *fd,
						int *output);

// builtin
int					builtin_echo(char **args);
int					builtin_cd(char **args, t_list *env);
int					cd_command(t_list *env, char *path);
void				modify_env(t_list *env, char *name, char *new);
int					builtin_pwd(void);
int					builtin_export(char **args, t_list *env);
bool				name_validity(const char *s);
void				modify_var(char *s, t_list **env_list);
int					add_back(char *s, t_list **env);
void				print_invalid(char *arg);
int					builtin_unset(char **args, t_list *env);
void				unset_var(char *arg, t_list **env);
void				link_and_free(t_list *prev, t_list *current);
int					builtin_env(t_list *envm, char **args);
int					builtin_exit(char **args);
void				free_cmd_list(void);
int					valid_input(char **args);
int					is_nbr(char *str);

#endif