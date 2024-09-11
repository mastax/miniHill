/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_executer_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:22:39 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/11 20:22:40 by elel-bah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

void	free_env(t_env *env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (i < env->count)
	{
		free(env->env_vars[i]);
		i++;
	}
	free(env->env_vars);
	free(env);
}

int	is_builtin(const char *cmd)
{
	int			i;
	const char	*builtins[8];

	i = 0;
	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = NULL;
	if (cmd == NULL)
		return (0);
	while (builtins[i])
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}
