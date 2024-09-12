/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_executer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 10:12:11 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/11 19:31:48 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

int	execute_builtin_p(t_arg *cmd, t_env *env, int *exit_status)
{
	if (cmd->arg == NULL || cmd->arg[0] == NULL)
		return (0);
	if (ft_strcmp(cmd->arg[0], "cd") == 0)
		return (ft_change_dir(cmd->arg, env, exit_status));
	else if (ft_strcmp(cmd->arg[0], "exit") == 0)
		return (ft_exit(cmd->arg, exit_status));
	else if (ft_strcmp(cmd->arg[0], "export") == 0)
		return (ft_exports(env, cmd->arg, exit_status));
	else if (ft_strcmp(cmd->arg[0], "unset") == 0)
		return (ft_unsets(env, cmd->arg, exit_status));
	return (1);
}

int	execute_builtin_ch(t_arg *cmd, t_env *env, int *exit_status)
{
	if (cmd->arg == NULL || cmd->arg[0] == NULL)
		return (0);
	if (ft_strcmp(cmd->arg[0], "cd") == 0)
		return (ft_change_dir(cmd->arg, env, exit_status));
	else if (ft_strcmp(cmd->arg[0], "echo") == 0)
		return (ft_echo(cmd->arg));
	else if (ft_strcmp(cmd->arg[0], "env") == 0)
	{
		if (cmd->arg[1] == NULL)
			return (ft_env(env));
		ft_putstr_fd("env: ", 2);
		ft_putstr_fd(cmd->arg[1], 2);
		ft_putstr_fd(": shouldn't work with no options or arguments\n", 2);
		return (1);
	}
	else if (ft_strcmp(cmd->arg[0], "exit") == 0)
		return (ft_exit(cmd->arg, exit_status));
	else if (ft_strcmp(cmd->arg[0], "export") == 0)
		return (ft_exports(env, cmd->arg, exit_status));
	else if (ft_strcmp(cmd->arg[0], "pwd") == 0)
		return (pwd(STDOUT_FILENO));
	else if (ft_strcmp(cmd->arg[0], "unset") == 0)
		return (ft_unsets(env, cmd->arg, exit_status));
	return (1);
}
