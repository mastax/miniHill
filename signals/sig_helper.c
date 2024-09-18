/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 14:29:22 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/15 14:29:23 by elel-bah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

int	get_sigint(int n)
{
	static int	v;

	v = 0;
	if (n != -500)
		v = n;
	return (v);
}

int	get_sigquit(int n)
{
	static int	v;

	v = 0;
	if (n != -500)
		v = n;
	return (v);
}

int	get_pid(int n)
{
	static int	v;

	v = 0;
	if (n != -500)
		v = n;
	return (v);
}

int	get_exit_status(int n)
{
	static int	v;

	v = 0;
	if (n != -500)
		v = n;
	return (v);
}

int	get_in_heredoc(int n)
{
	static int	v;

	v = 0;
	if (n != -500)
		v = n;
	return (v);
}
