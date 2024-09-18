/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 18:17:59 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/09/16 18:24:09 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

t_env	*allocate_env(int count)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->env_vars = malloc(sizeof(char *) * (count + 1));
	if (!env->env_vars)
	{
		free(env);
		return (NULL);
	}
	env->count = count;
	return (env);
}

static t_env	*init_env(char **envp, int count)
{
	t_env	*env;

	(void)envp;
	if (count == 0)
		return (create_env_v_i(5));
	env = allocate_env(count);
	return (env);
}

static int	duplicate_env_vars(t_env *env, char **envp, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		env->env_vars[i] = ft_strdup(envp[i]);
		if (!env->env_vars[i])
			return (i);
		i++;
	}
	return (count);
}

static void	free_env_vars(t_env *env, int count)
{
	while (--count >= 0)
		free(env->env_vars[count]);
	free(env->env_vars);
	free(env);
}

t_env	*create_env(char **envp)
{
	t_env	*env;
	int		count;
	int		i;

	count = 0;
	while (envp[count])
		count++;
	env = init_env(envp, count);
	if (!env)
		return (NULL);
	if (count == 0)
		return (env);
	i = duplicate_env_vars(env, envp, count);
	if (i < count)
	{
		free_env_vars(env, i);
		return (NULL);
	}
	env->env_vars[count] = NULL;
	return (env);
}
