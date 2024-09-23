/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 10:03:08 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/09/12 20:38:07 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

int	check_pipe(t_token *token, t_token *next_token)
{
	if (token->type == PIPE && next_token->type == PIPE)
		return (1);
	else
		return (0);
}

int	check_redirect(t_token *token, t_token *next_token)
{
	if (token->type == RED_IN || token->type == RED_OUT
		|| token->type == HER_DOC || token->type == APPEND)
	{
		if (next_token->type == RED_IN || next_token->type == RED_OUT
			|| next_token->type == HER_DOC || next_token->type == APPEND)
			return (1);
		return (0);
	}
	else
		return (0);
}

int	ft_check_error(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == PIPE)
	{
		ft_putstr_fd("syntax error near unexpected token '", 2);
		ft_putstr_fd(token->content, 2);
		ft_putstr_fd("'\n", 2);
		return (258);
	}
	while (token->next)
	{
		if (check_pipe(token, token->next) != 0
			|| check_redirect(token, token->next) != 0)
		{
			ft_putstr_fd("syntax error near unexpected token '", 2);
			ft_putstr_fd(token->next->content, 2);
			ft_putstr_fd("'\n", 2);
			return (258);
		}
		token = token->next;
	}
	if (token->type != WORD)
		return (ft_putstr_fd("syntax error near unexpected token 'newline'\n"
				, 2), 258);
	return (0);
}
