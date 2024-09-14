/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_arg_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 20:24:04 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/09/12 20:25:47 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

int	count_args_red(t_token *token, int type, int i)
{
	int		j;
	t_token	*tmp;

	j = 0;
	tmp = token;
	while (tmp)
	{
		if (tmp->type == WORD)
			i++;
		else if (tmp->type == HER_DOC || tmp->type == APPEND
			|| tmp->type == RED_IN || tmp->type == RED_OUT)
		{
			j += 2;
			tmp = tmp->next;
		}
		else if (tmp->type == PIPE)
			break ;
		tmp = tmp->next;
	}
	if (type == 1)
		return (i);
	else if (type == 2)
		return (j);
	return (0);
}

char	**ft_handl_args(char *content, char **arv, int type)
{
	int	i;

	i = 0;
	if (type == 1)
	{
		arv[0] = ft_strdup(content);
		if (arv[0] == NULL)
			return (NULL);
		arv[1] = NULL;
	}
	else if (type == 2)
	{
		while (arv[i] != NULL)
			i++;
		arv[i] = ft_strdup(content);
		if (arv[i] == NULL)
			return (NULL);
		arv[i + 1] = NULL;
	}
	return (arv);
}
