/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 11:02:58 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/16 22:18:20 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

static int	handle_existing_var_append(t_env *env, char *name,
	char *value, int i)
{
	char	*old_value;
	char	*new_value;
	char	*tmp;

	old_value = ft_strchr(env->env_vars[i], '=');
	if (old_value)
	{
		old_value++;
		tmp = ft_concat(old_value, value);
		new_value = ft_concat(name, "=");
		free(env->env_vars[i]);
		env->env_vars[i] = ft_concat(new_value, tmp);
		free(new_value);
		free(tmp);
	}
	else
	{
		tmp = ft_concat(name, "=");
		free(env->env_vars[i]);
		env->env_vars[i] = ft_concat(tmp, value);
		free(tmp);
	}
	return (1);
}

static int	handle_existing_var(t_env *env, char *name, char *value,
		int is_append)
{
	int		i;
	char	*temp;

	i = 0;
	while (env->env_vars[i] && ft_strncmp(env->env_vars[i], name,
			ft_strlen(name)))
		i++;
	if (!env->env_vars[i])
		return (0);
	if (!value)
		return (1);
	if (is_append)
		return (handle_existing_var_append(env, name, value, i));
	temp = ft_concat(name, "=");
	free(env->env_vars[i]);
	env->env_vars[i] = ft_concat(temp, value);
	free(temp);
	return (1);
}

static int	find_and_update_var(t_env *env, char *name, char *value,
		int is_append)
{
	int		i;
	size_t	name_len;
	char	*new_var;
	char	*temp;

	i = 0;
	name_len = ft_strlen(name);
	while (i < env->count)
	{
		if (ft_strncmp(env->env_vars[i], name, name_len) == 0
			&& (env->env_vars[i][name_len] == '='
			|| env->env_vars[i][name_len] == '\0'))
			return (handle_existing_var(env, name, value, is_append));
		i++;
	}
	if (is_append)
	{
		temp = ft_concat(name, "=");
		new_var = ft_concat(temp, value);
		free(temp);
		if (!append_new_var(env, new_var))
			return (free(new_var), 0);
		return (free(new_var), 1);
	}
	return (0);
}

int	ft_export(t_env *env, char *s)
{
	char	*name;
	char	*value;
	int		is_append;

	is_append = 0;
	if (check_format(s) == 1)
		return (2);
	parse_export_string(s, &name, &value, &is_append);
	if (!find_and_update_var(env, name, value, is_append))
	{
		if (!append_new_var(env, s))
		{
			free(name);
			return (0);
		}
	}
	free(name);
	return (1);
}

int	ft_exports(t_env *env, char **args, int *exit_status)
{
	int	i;
	int	command_return;

	*exit_status = 0;
	if (!args[1] || (args[1][0] == '\0' && !args[2]))
	{
		print_sorted_env(env);
		return (1);
	}
	i = 1;
	if (args[1][0] == '\0')
		i = 2;
	while (args[i])
	{
		command_return = ft_export(env, args[i]);
		if (command_return == 0)
			return (0);
		else if (command_return == 2)
			*exit_status = 1;
		i++;
	}
	if (*exit_status == 1)
		return (get_exit_status(1), 1);
	return (get_exit_status(0), 1);
}
