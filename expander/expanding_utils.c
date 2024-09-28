/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 10:00:52 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/09/28 09:57:16 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

int	ft_name_len(char *var, int i)
{
	int	len;

	len = 0;
	if (var[i] > '0' && var[i] <= '9')
		return (1);
	while (var[i + len] != '\0' && (is_valid_var(var[i + len]) == 1))
		len++;
	return (len);
}

t_token	*list_befor_last(t_token *list)
{
	t_token	*tmp;

	tmp = list;
	if (!tmp)
		return (NULL);
	while (tmp->next->next)
		tmp = tmp->next;
	return (tmp);
}

void	update_token_list(t_token **token, t_token *now, t_token *new_tokens
, t_token *next_token)
{
	t_token	*tmp;

	tmp = *token;
	if (tmp == now)
		*token = new_tokens;
	else
	{
		while (tmp->next != now)
			tmp = tmp->next;
		tmp->next = new_tokens;
	}
	tmp = new_tokens;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = next_token;
}

int	cheak_exp_case(t_token *t)
{
	int	i;

	i = 0;
	while (t->befor_exp[i] != '\0')
	{
		if (t->befor_exp[i] == '=' && t->befor_exp[i + 1] == '$')
			return (1);
		i++;
	}
	return (0);
}

int	ft_handl_spichel_cond(t_token **token, t_token *now, t_token *next_token,
t_type *prv_type)
{
	t_token	*new_tokens;
	t_token	*tmp;
	bool	had_to_rmove;

	new_tokens = NULL;
	had_to_rmove = false;
	if (cheak_exp_case(now) == 1)
		return (1);
	if ((*prv_type == APPEND || *prv_type == RED_IN || *prv_type == RED_OUT))
		return (ambiguous_error(now->content), -1);
	if (had_qout(now->befor_exp) == 0)
		had_to_rmove = true;
	if (get_token(&new_tokens, now->content, 2) == -1)
		return (-1);
	tmp = new_tokens;
	while (tmp && had_to_rmove == true)
	{
		if (ft_remove_quotes(tmp, 0, 0) == -1)
			return (-1);
		tmp = tmp->next;
	}
	update_token_list(token, now, new_tokens, next_token);
	free(now->content);
	free(now->befor_exp);
	return (free(now), 0);
}
