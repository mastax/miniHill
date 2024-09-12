/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 13:30:12 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/12 13:30:14 by elel-bah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

void	child_process(int pipefd[2], const char *delimiter,
		t_env *env, t_fd_tracker *fd_tracker)
{
	char	*line;

	close(pipefd[0]);
	untrack_fd(fd_tracker, pipefd[0]);
	line = read_line();
	while (line != NULL)
	{
		if (ft_strcmp(line, delimiter) == 0)
			break ;
		if (fd_tracker->qout == 0)
			if (expand_variable(&line, env) == -1)
				exit(1);
		write_to_pipe(pipefd, line);
		free(line);
		line = read_line();
	}
	close(pipefd[1]);
	untrack_fd(fd_tracker, pipefd[1]);
}

void	parent_process(int pipefd[2], pid_t pid, t_fd_tracker *fd_tracker)
{
	int	status;

	(void)pid;
	close(pipefd[1]);
	untrack_fd(fd_tracker, pipefd[1]);
	waitpid(pid, &status, 0);
}

int	check_if_qoutes(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

static int	process_heredocs(t_setup_context *ctx, t_fd_tracker *fd_tracker)
{
	t_arg	*current_cmd;
	int		heredoc_count;

	current_cmd = ctx->cmd;
	while (current_cmd)
	{
		heredoc_count = count_heredocs(current_cmd->red);
		if (heredoc_count > 0)
		{
			current_cmd->heredoc_fds = handle_heredocs(current_cmd->red,
					heredoc_count, ctx->env, fd_tracker);
			if (!current_cmd->heredoc_fds)
			{
				restore_io(ctx->io);
				return (1);
			}
		}
		current_cmd = current_cmd->next;
	}
	return (0);
}

int	setup_and_handle_heredocs(t_setup_context *ctx)
{
	t_fd_tracker	fd_tracker;

	ft_memset(&fd_tracker, 0, sizeof(fd_tracker));
	*(ctx->command_count) = count_commands(ctx->cmd);
	*(ctx->pipe_count) = *(ctx->command_count) - 1;
	save_original_io(ctx->io);
	if (setup_pipes(*(ctx->pipe_count), ctx->pipe_fds, &fd_tracker) != 0)
	{
		restore_io(ctx->io);
		return (1);
	}
	return (process_heredocs(ctx, &fd_tracker));
}

// int	setup_and_handle_heredocs(t_setup_context *ctx)
// {
// 	t_fd_tracker	fd_tracker = {0};
// 	t_arg			*current_cmd;
// 	int				heredoc_count;

// 	*(ctx->command_count) = count_commands(ctx->cmd);
// 	*(ctx->pipe_count) = *(ctx->command_count) - 1;
// 	save_original_io(ctx->io);
// 	if (setup_pipes(*(ctx->pipe_count), ctx->pipe_fds, &fd_tracker) != 0)
// 	{
// 		restore_io(ctx->io);
// 		return (1);
// 	}
// 	// Process all heredocs
// 	current_cmd = ctx->cmd;
// 	while (current_cmd)
// 	{
// 		heredoc_count = count_heredocs(current_cmd->red);
// 		if (heredoc_count > 0)
// 		{
// 			current_cmd->heredoc_fds = handle_heredocs(current_cmd->red,
// 					heredoc_count, ctx->env, &fd_tracker);
// 			if (!current_cmd->heredoc_fds)
// 			{
// 				restore_io(ctx->io);
// 				return (1);
// 			}
// 		}
// 		current_cmd = current_cmd->next;
// 	}
// 	return (0);
// }
