/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 18:37:39 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/14 18:37:41 by elel-bah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

static int	handle_parent_process(t_command_context1 *ctx, t_arg *current_cmd,
		pid_t *pids, int cmd_index)
{
	if (cmd_index >= MAX_PIPES) {
        fprintf(stderr, "Error: Too many piped commands\n");
        restore_io(ctx->io);
        return 0;
    }
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
		execute_child_process_part1(ctx, current_cmd);
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

static void	process_single_command(t_process_data *data, t_arg *current_cmd)
{
	int	is_builtins;
	int	is_parent_builtin;

	is_builtins = current_cmd->arg != NULL
		&& is_builtin(current_cmd->arg[0]);
	is_parent_builtin = is_builtins
		&& (ft_strcmp(current_cmd->arg[0], "cd") == 0
			|| ft_strcmp(current_cmd->arg[0], "exit") == 0
			|| ft_strcmp(current_cmd->arg[0], "export") == 0
			|| ft_strcmp(current_cmd->arg[0], "unset") == 0);
	if (is_parent_builtin && data->ctx->pipe_count == 0)
		handle_parent_builtin(data->ctx, current_cmd);
	else
		data->child_count += handle_parent_process(data->ctx, current_cmd,
				data->pids, data->cmd_index);
	get_pid(data->pids[data->cmd_index]);
}

int	process_commands(t_command_context1 *ctx, pid_t *pids)
{
	t_process_data	data;
	t_arg			*current_cmd;

	data.ctx = ctx;
	data.pids = pids;
	data.child_count = 0;
	data.cmd_index = 0;
	current_cmd = ctx->cmd;
	while (current_cmd)
	{
		process_single_command(&data, current_cmd);
		current_cmd = current_cmd->next;
		data.cmd_index++;
	}
	return (data.child_count);
}
