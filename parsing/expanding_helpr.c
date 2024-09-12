/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_helpr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 16:23:54 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/09/11 18:30:57 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

void	ft_handl_exp_qout(t_token *t)
{
	int j;
	int i;

	i = 0;
	while (t->content[i] != '\0')
	{
		if (t->content[i] == '"')
		{
			j = i;
			i++;
			while (t->content[i] != '\0' && t->content[i] != '"')
				i++;
			if (t->qout_rm == true && (t->content[j] == '"' && t->content[i] == '"'))
			{
				t->content[j] = 127;
				t->content[i] = 127;
			}
		}
		if (t->content[i] != '\0')
			i++;
	}
}
