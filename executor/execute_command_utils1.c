/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:31:32 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/11 20:31:34 by elel-bah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

void	configure_child_io(t_command_context1 *ctx,
		t_arg *current_cmd, int cmd_index)
{
	int	i;

	if (cmd_index > 0)
		dup2(ctx->pipe_fds[cmd_index - 1][0], STDIN_FILENO);
	if (cmd_index < ctx->pipe_count)
		dup2(ctx->pipe_fds[cmd_index][1], STDOUT_FILENO);
	i = 0;
	while (i < ctx->pipe_count)
	{
		close(ctx->pipe_fds[i][0]);
		close(ctx->pipe_fds[i][1]);
		i++;
	}
	if (current_cmd->heredoc_fds)
	{
		i = 0;
		while (i < count_heredocs(current_cmd->red))
		{
			dup2(current_cmd->heredoc_fds[i], STDIN_FILENO);
			close(current_cmd->heredoc_fds[i]);
			i++;
		}
	}
	if (apply_redirections(current_cmd->red) == -1)
		exit(1);
}
//=-=-=-+

static void	execute_child_process_part2(t_command_context1 *ctx,
		t_arg *current_cmd)
{
	char	*cmd_path;

	cmd_path = ft_get_path(current_cmd->arg[0], ctx->exit_status);
	if (cmd_path == NULL)
		cmd_path = find_command(current_cmd->arg[0], ctx->env->env_vars);
	if (!cmd_path)
	{
		ft_putstr_fd(current_cmd->arg[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	execve(cmd_path, current_cmd->arg, ctx->env->env_vars);
	perror("execve");
	exit(1);
}

void	execute_child_process_part1(t_command_context1 *ctx,
		t_arg *current_cmd)
{
	int	builtin_result;

	if (current_cmd->arg == NULL || current_cmd->arg[0] == NULL)
		exit(0);
	if (is_builtin(current_cmd->arg[0]))
	{
		builtin_result = execute_builtin_ch(current_cmd, ctx->env,
				ctx->exit_status);
		exit(builtin_result);
	}
	if (current_cmd->arg[0][0] == '\0')
	{
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	execute_child_process_part2(ctx, current_cmd);
}

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
