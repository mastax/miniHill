/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:45:19 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/11 20:45:21 by elel-bah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

static void	init_setup_context(t_setup_context *setup_ctx,
		t_command_context *context, t_execution_data *exec_data)
{
	setup_ctx->cmd = context->cmd;
	setup_ctx->env = context->env;
	setup_ctx->io = &exec_data->io;
	setup_ctx->pipe_fds = exec_data->pipe_fds;
	setup_ctx->command_count = &exec_data->command_count;
	setup_ctx->pipe_count = &exec_data->pipe_count;
}

static void	init_command_context1(t_command_context1 *ctx,
		t_command_context *context, t_execution_data *exec_data)
{
	ctx->cmd = context->cmd;
	ctx->env = context->env;
	ctx->exit_status = context->exit_status;
	ctx->io = &exec_data->io;
	ctx->pipe_fds = exec_data->pipe_fds;
	ctx->command_count = exec_data->command_count;
	ctx->pipe_count = exec_data->pipe_count;
}

void	initialize_loop_iteration(t_token **tokens)
{
	*tokens = NULL;
	get_pid(0);
	get_sigint(0);
	signal(SIGINT, sig_int);
	signal(SIGQUIT, sig_quit);
}

int	execute_commands(t_command_context1 *ctx)
{
	pid_t	pids[MAX_COMMANDS];
	int		child_count;
	int		result;

	child_count = process_commands(ctx, pids);
	cleanup_pipes(ctx);
	result = 0;
	if (child_count > 0)
		result = wait_for_children(pids, child_count, ctx->exit_status);
	restore_io(ctx->io);
	return (result);
}

int	execute_command(t_command_context *context)
{
	t_execution_data	exec_data;
	t_setup_context		setup_ctx;
	t_command_context1	cmd_ctx;
	int					result;

	ft_memset(&exec_data, 0, sizeof(exec_data));
	exec_data.command_count = 0;
	exec_data.pipe_count = 0;
	init_setup_context(&setup_ctx, context, &exec_data);
	if (setup_and_handle_heredocs(&setup_ctx) != 0)
	{
		close_all_fds(context->fd_tracker);
		return (1);
	}
	init_command_context1(&cmd_ctx, context, &exec_data);
	result = execute_commands(&cmd_ctx);
	cleanup_heredoc_fds(context->cmd, context->fd_tracker);
	return (result);
}
