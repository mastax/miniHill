/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 11:20:45 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/24 15:49:30 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

int	unset_env_value(t_env *env, const char *key)
{
	int		i;
	int		j;
	size_t	key_len;

	i = -1;
	key_len = ft_strlen(key);
	while (++i < env->count)
	{
		if (ft_strncmp(env->env_vars[i], key, key_len) == 0
			&& (env->env_vars[i][key_len] == '='
			|| env->env_vars[i][key_len] == '\0'))
		{
			free(env->env_vars[i]);
			j = i - 1;
			while (++j < env->count - 1)
				env->env_vars[j] = env->env_vars[j + 1];
			env->count--;
			env->env_vars[env->count] = NULL;
			return (1);
		}
	}
	return (0);
}

int	ft_unsets(t_env *env, char **args, int *exit_status)
{
	int	i;
	int	track_err;

	i = 1;
	track_err = 0;
	while (args[i])
	{
		if (check_format(args[i], 2) == 1)
			track_err = 1;
		else
			unset_env_value(env, args[i]);
		i++;
	}
	*exit_status = track_err;
	get_exit_status(*exit_status);
	return (1);
}
