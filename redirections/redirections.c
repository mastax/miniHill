/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 22:08:46 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/12 22:08:48 by elel-bah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

static int	handle_output_redirection(const char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_input_redirection(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	process_redirection(char **red, int *i)
{
	if (ft_strcmp(red[*i], ">") == 0)
		return (handle_output_redirection(red[*i + 1], O_WRONLY
				| O_CREAT | O_TRUNC));
	else if (ft_strcmp(red[*i], ">>") == 0)
		return (handle_output_redirection(red[*i + 1], O_WRONLY
				| O_CREAT | O_APPEND));
	else if (ft_strcmp(red[*i], "<") == 0)
		return (handle_input_redirection(red[*i + 1]));
	return (-1);
}

int	apply_redirections(char **red)
{
	int	i;

	i = 0;
	if (!red || !red[0])
		return (0);
	while (red[i])
	{
		if (ft_strcmp(red[i], ">") == 0 || ft_strcmp(red[i], ">>") == 0
			|| ft_strcmp(red[i], "<") == 0)
		{
			if (process_redirection(red, &i) == -1)
				return (-1);
			i += 2;
		}
		else
			i++;
	}
	return (0);
}
