/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leaks_fd_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:52:33 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/11 20:52:34 by elel-bah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

void	track_fd(t_fd_tracker *tracker, int fd)
{
	if (fd >= 0 && fd < MAX_FD)
		tracker->fd_table[fd] = 1;
}

void	untrack_fd(t_fd_tracker *tracker, int fd)
{
	if (fd >= 0 && fd < MAX_FD)
		tracker->fd_table[fd] = 0;
}

void	close_all_fds(t_fd_tracker *tracker)
{
	int	fd;

	fd = 3;
	while (fd < MAX_FD)
	{
		if (tracker->fd_table[fd])
		{
			close(fd);
			untrack_fd(tracker, fd);
		}
		fd++;
	}
}
