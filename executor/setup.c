/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:04:32 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/11 21:04:33 by elel-bah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

int	count_commands(t_arg *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

int	wait_for_children(pid_t *pids, int command_count, int *exit_status)
{
	int	status;
	int	i;

	i = 0;
	while (i < command_count)
	{
		if (waitpid(pids[i], &status, 0) == -1)
		{
			perror("waitpid");
			return (1);
		}
		if (WIFEXITED(status))
			*exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*exit_status = 128 + WTERMSIG(status);
		if (i == command_count - 1)
			get_exit_status(*exit_status);
		i++;
	}
	return (*exit_status);
}

int	setup_pipes(int pipe_count, int pipe_fds[][2], t_fd_tracker *fd_tracker)
{
	int	i;

	(void)fd_tracker;
	i = 0;
	while (i < pipe_count)
	{
		if (pipe(pipe_fds[i]) == -1)
		{
			perror("pipe");
			return (1);
		}
		track_fd(fd_tracker, pipe_fds[i][0]);
		track_fd(fd_tracker, pipe_fds[i][1]);
		i++;
	}
	return (0);
}

void	setup_child_process(child_setup_params *params)
{
	int	i;

	if (params->cmd_index > 0)
		dup2(params->pipe_fds[params->cmd_index - 1][0], STDIN_FILENO);
	if (params->cmd_index < params->pipe_count)
		dup2(params->pipe_fds[params->cmd_index][1], STDOUT_FILENO);
	i = 0;
	while (i < params->pipe_count)
	{
		close(params->pipe_fds[i][0]);
		close(params->pipe_fds[i][1]);
		i++;
	}
	if (params->heredoc_fds && params->heredoc_count > 0)
	{
		dup2(params->heredoc_fds[params->heredoc_count - 1], STDIN_FILENO);
		i = 0;
		while (i < params->heredoc_count)
		{
			close(params->heredoc_fds[i]);
			i++;
		}
	}
}
