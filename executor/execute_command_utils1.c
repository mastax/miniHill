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

static void	configure_child_io(t_command_context1 *ctx,
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

static void	execute_child_process(t_command_context1 *ctx, t_arg *current_cmd)
{
	int		builtin_result;
	char	*cmd_path;

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

static int	handle_parent_process(t_command_context1 *ctx, t_arg *current_cmd,
		pid_t *pids, int cmd_index)
{
	pids[cmd_index] = fork();
	if (pids[cmd_index] == -1)
	{
		perror("fork");
		restore_io(ctx->io);
		return (0);
	}
	else if (pids[cmd_index] == 0)
	{
		configure_child_io(ctx, current_cmd, cmd_index);
		execute_child_process(ctx, current_cmd);
	}
	restore_io(ctx->io);
	return (1);
}

static void	handle_parent_builtin(t_command_context1 *ctx, t_arg *current_cmd)
{
	if (apply_redirections(current_cmd->red) == -1)
	{
		restore_io(ctx->io);
		return ;
	}
	execute_builtin_p(current_cmd, ctx->env, ctx->exit_status);
	restore_io(ctx->io);
}

int	process_commands(t_command_context1 *ctx, pid_t *pids)
{
	int		is_builtins;
	int		is_parent_builtin;
	int		child_count;
	int		cmd_index;
	t_arg	*current_cmd;

	child_count = 0;
	cmd_index = 0;
	current_cmd = ctx->cmd;
	while (current_cmd)
	{
		is_builtins = current_cmd->arg != NULL
			&& is_builtin(current_cmd->arg[0]);
		is_parent_builtin = is_builtins
			&& (ft_strcmp(current_cmd->arg[0], "cd") == 0
				|| ft_strcmp(current_cmd->arg[0], "exit") == 0
				|| ft_strcmp(current_cmd->arg[0], "export") == 0
				|| ft_strcmp(current_cmd->arg[0], "unset") == 0);
		if (is_parent_builtin && ctx->pipe_count == 0)
			handle_parent_builtin(ctx, current_cmd);
		else
			child_count += handle_parent_process(ctx, current_cmd,
					pids, cmd_index);
		get_pid(pids[cmd_index]);
		current_cmd = current_cmd->next;
		cmd_index++;
	}
	return (child_count);
}
