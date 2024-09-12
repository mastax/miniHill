/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 20:41:17 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/12 20:41:18 by elel-bah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

static char	*append_buffer(char *line, char *buffer,
		int bytes_read, int *total_size)
{
	char	*new_line;
	int		new_size;

	new_size = *total_size + bytes_read + 1;
	new_line = malloc(new_size);
	if (!new_line)
	{
		free(line);
		return (NULL);
	}
	if (line)
	{
		ft_memcpy(new_line, line, *total_size);
		free(line);
	}
	ft_memcpy(new_line + *total_size, buffer, bytes_read);
	*total_size = new_size - 1;
	new_line[*total_size] = '\0';
	return (new_line);
}

static char	*handle_error(char *line)
{
	free(line);
	return (NULL);
}

char	*read_line(void)
{
	char	*line;
	char	buffer[BUFFER_SIZE];
	int		bytes_read;
	int		total_size;

	line = NULL;
	total_size = 0;
	bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		line = append_buffer(line, buffer, bytes_read, &total_size);
		if (!line)
			return (handle_error(NULL));
		if (line[total_size - 1] == '\n')
		{
			line[total_size - 1] = '\0';
			return (line);
		}
		bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
	}
	if (bytes_read == -1 || (bytes_read == 0 && total_size == 0))
		return (handle_error(line));
	return (line);
}
