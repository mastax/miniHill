#include "../../mini_shell.h"

char	*get_env_value(t_env *env, const char *key)
{
	int	key_len;
	int	i;

	key_len = ft_strlen(key);
	i = 0;
	while (i < env->count)
	{
		if (ft_strncmp(env->env_vars[i], key, key_len) == 0
			&& env->env_vars[i][key_len] == '=')
			return (env->env_vars[i] + key_len + 1);
		i++;
	}
	return (NULL);
}

int	change_to_home(t_env *env)
{
	char	*home;

	home = get_env_value(env, "HOME");
	if (!home)
	{
		ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
		return (1);
	}
	return (chdir(home));
}

int	change_to_previous(t_env *env)
{
	char	*oldpwd;

	oldpwd = get_env_value(env, "OLDPWD");
	if (!oldpwd)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
		return (1);
	}
	ft_putendl_fd(oldpwd, STDOUT_FILENO);
	return (chdir(oldpwd));
}
