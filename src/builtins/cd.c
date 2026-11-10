#include "minishell.h"

void	modify_env(t_list *env, char *name, char *new)
{
	int		len;
	t_env	*env_var;

	len = ft_strlen(name);
	while (env)
	{
		env_var = (t_env *)env->content;
		if (ft_strncmp(env_var->name, name, len + 1) == 0)
		{
			free(env_var->value);
			env_var->value = ft_strdup(new);
			if (!env_var->value)
				return ;
			break ;
		}
		env = env->next;
	}
}

int	cd_command(t_list *env, char *path)
{
	char	*old_pwd;
	char	*pwd;

	old_pwd = getcwd(NULL, 0);
	modify_env(env, "OLDPWD", old_pwd);
	free(old_pwd);
	if (chdir(path) != 0)
		return (perror("cd"), 1);
	pwd = getcwd(NULL, 0);
	modify_env(env, "PWD", pwd);
	free(pwd);
	return (0);
}

int	builtin_cd(char **args, t_list *env)
{
	int	nargs;

	nargs = 0;
	while (args[nargs])
		nargs++;
	if (nargs > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	else
	{
		if (args[1] == NULL)
			return (cd_command(env, "/home"));
		else
			return (cd_command(env, args[1]));
	}
	return (1);
}
