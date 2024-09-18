/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 15:04:58 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/12 15:05:00 by elel-bah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

static char	*get_heredoc_delimiter(char **red, int index)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (red && red[i])
	{
		if (ft_strcmp(red[i], "<<") == 0)
		{
			if (count == index && red[i + 1])
				return (ft_strdup(red[i + 1]));
			count++;
		}
		i++;
	}
	return (NULL);
}

int	process_delimiter(char **red, int i, t_delimiter_info *info)
{
	int	j;

	j = 0;
	*(info->delimiter) = get_heredoc_delimiter(red, i);
	if (!*(info->delimiter))
		return (0);
	while ((*info->delimiter)[j])
	{
		if ((*info->delimiter)[j] == '$' && ((*info->delimiter)[j + 1] == '"'
			|| (*info->delimiter)[j + 1] == '\''))
		{
			*(info->delimiter) = ft_remove_char(*(info->delimiter), j);
			if (!*(info->delimiter))
				return (0);
		}
		j++;
	}
	info->tracker->qout = check_if_qoutes(*(info->delimiter));
	if (info->tracker->qout == 1)
		*(info->processed_delimiter) = ft_remove_quotes1(*(info->delimiter));
	else
		*(info->processed_delimiter) = *(info->delimiter);
	return (*(info->processed_delimiter) != NULL);
}

static int	setup_heredoc_process(int pipefd[2],
		t_fd_tracker *tracker, pid_t *pid)
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	track_fd(tracker, pipefd[0]);
	track_fd(tracker, pipefd[1]);
	get_in_heredoc(1);
	*pid = fork();
	if (*pid == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		untrack_fd(tracker, pipefd[0]);
		untrack_fd(tracker, pipefd[1]);
		get_in_heredoc(0);
		return (-1);
	}
	return (0);
}

int	create_heredoc(const char *delimiter, t_env *env, t_fd_tracker *tracker)
{
	int		pipefd[2];
	pid_t	pid;

	if (setup_heredoc_process(pipefd, tracker, &pid) == -1)
		return (-1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		child_process(pipefd, delimiter, env, tracker);
		exit(0);
	}
	else
	{
		parent_process(pipefd, pid, tracker);
		get_in_heredoc(0);
		untrack_fd(tracker, pipefd[1]);
		return (pipefd[0]);
	}
}

int	*handle_heredocs(char **red, int count, t_env *env, t_fd_tracker *tracker)
{
	int					*heredoc_fds;
	char				*delimiter;
	char				*processed_delimiter;
	int					i;
	t_delimiter_info	info;

	heredoc_fds = malloc(sizeof(int) * count);
	if (!heredoc_fds)
		return (NULL);
	i = 0;
	info.delimiter = &delimiter;
	info.processed_delimiter = &processed_delimiter;
	info.tracker = tracker;
	while (i < count)
	{
		if (!process_delimiter(red, i, &info))
			return (cleanup_and_return(heredoc_fds, i, tracker), NULL);
		heredoc_fds[i] = create_heredoc(processed_delimiter, env, tracker);
		free(processed_delimiter);
		if (heredoc_fds[i] == -1)
			return (cleanup_and_return(heredoc_fds, i, tracker), NULL);
		i++;
	}
	return (heredoc_fds);
}
