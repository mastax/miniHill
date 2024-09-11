/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 11:19:55 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/11 11:19:57 by elel-bah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

int	pwd(int fd)
{
	char	current_wd[PATH_MAX];

	if (getcwd(current_wd, sizeof(current_wd)) == NULL)
	{
		ft_putendl_fd(current_wd, STDOUT_FILENO);
		return (1);
	}
	ft_putendl_fd(current_wd, fd);
	return (0);
}
