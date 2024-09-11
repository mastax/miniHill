/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 10:20:18 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/11 10:20:20 by elel-bah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

int	ft_env(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->count)
	{
		if (ft_strchr(env->env_vars[i], '=') != NULL)
		{
			ft_putstr_fd(env->env_vars[i], STDOUT_FILENO);
			write(STDOUT_FILENO, "\n", 1);
		}
		i++;
	}
	return (0);
}
