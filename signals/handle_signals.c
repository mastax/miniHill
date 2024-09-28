/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 10:52:04 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/27 16:20:03 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

void	sig_int(int code)
{
	(void)code;
	if (get_in_heredoc(-500))
	{
		write(STDERR_FILENO, "\n", 1);
		get_exit_status(1);
	}
	else if (get_pid(-500) == -1)
	{
		write(STDERR_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		get_exit_status(1);
	}
	else
	{
		write(STDERR_FILENO, "\n", 1);
	}
}

void	sig_quit(int code)
{
	char	*nbr;

	nbr = ft_itoa(code);
	if (get_pid(-500) != -1)
	{
		get_exit_status(131);
		get_sigquit(1);
	}
	free(nbr);
}
