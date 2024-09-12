/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:19:18 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/11 20:19:21 by elel-bah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

void	cleanup_parent_process(int *heredoc_fds, int heredoc_count)
{
	int	i;

	if (heredoc_fds)
	{
		i = 0;
		while (i < heredoc_count)
		{
			close(heredoc_fds[i]);
			i++;
		}
		free(heredoc_fds);
	}
}

void	cleanup_pipes(t_command_context1 *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->pipe_count)
	{
		close(ctx->pipe_fds[i][0]);
		close(ctx->pipe_fds[i][1]);
		i++;
	}
}

void	cleanup_command_heredocs(t_arg *cmd, t_fd_tracker *fd_tracker)
{
	int	i;
	int	heredoc_count;

	i = 0;
	heredoc_count = count_heredocs(cmd->red);
	while (i < heredoc_count)
	{
		if (cmd->heredoc_fds[i] > 2)
		{
			close(cmd->heredoc_fds[i]);
			untrack_fd(fd_tracker, cmd->heredoc_fds[i]);
		}
		i++;
	}
}

void	cleanup_heredoc_fds(t_arg *cmd, t_fd_tracker *fd_tracker)
{
	t_arg	*current_cmd;

	current_cmd = cmd;
	while (current_cmd)
	{
		if (current_cmd->heredoc_fds)
			cleanup_command_heredocs(current_cmd, fd_tracker);
		current_cmd = current_cmd->next;
	}
}
void	cleanup_and_return(int *heredoc_fds, int count, t_fd_tracker *tracker)
{
	int	j;

	j = 0;
	while (j < count)
	{
		close(heredoc_fds[j]);
		untrack_fd(tracker, heredoc_fds[j]);
		j++;
	}
	free(heredoc_fds);
}
