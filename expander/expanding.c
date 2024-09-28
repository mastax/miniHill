/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:55:47 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/09/27 22:04:15 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

int	had_qout(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '\'' || s[i] == '"')
			return (0);
		i++;
	}
	return (1);
}

int	skip_varname(char *s, int i)
{
	i++;
	if (s[i] > '0' && s[i] <= '9' && s[i + 1] != '\0')
		return (1);
	while (s[i] != '\0')
	{
		if (s[i] == ' ' || s[i] == '\t' || is_valid_var(s[i] == 0))
			return (1);
		i++;
	}
	return (0);
}

int	handle_spp_case(char *s)
{
	int		i;
	char	qout;

	i = 0;
	if ((s[0] == '\'' && s[1] == '\'') || (s[0] == '"' && s[1] == '"'))
		return (1);
	else if ((s[ft_strlen(s) - 1] == '\'' && s[ft_strlen(s) - 2] == '\'')
		|| (s[ft_strlen(s) - 1] == '"' && s[ft_strlen(s) - 2] == '"'))
		return (1);
	if (s[i] == '\'' || s[i] == '"')
	{
		qout = s[i];
		i++;
		while (s[i] != '\0' && s[i] != qout)
		{
			if (s[i] == qout || s[i] == '$' || s[i] != ' ' || s[i] != '\t')
				break ;
			i++;
		}
		if (s[i] == qout || s[i] != ' ' || s[i] != '\t')
			return (1);
		if (s[i] == '$')
			return (skip_varname(s, i));
	}
	return (0);
}

int	handle_expanded_var(t_token **token, t_type prv_type, int result)
{
	t_token	*t;

	t = *token;
	while (t)
	{
		if (ft_count_sp_tb(t->content) != ft_count_sp_tb(t->befor_exp))
		{
			result = ft_handl_spichel_cond(token, t, t->next, &prv_type);
			if (result == -1)
				return (-1);
			else if (result == 0)
			{
				prv_type = WORD;
				t = *token;
			}
			else if (result == 1)
				prv_type = t->type;
		}
		prv_type = t->type;
		t = t->next;
	}
	return (0);
}

int	expanding(t_token **token, t_env *env, int exit_status, t_type prv_type)
{
	t_token	*t;

	t = *token;
	while (t)
	{
		if (prv_type != HER_DOC)
		{
			if (ft_expand_variable(t, env, prv_type, 0) == -1)
				return (-1);
			if (ft_expand_exit_status(&t->content, exit_status, 0) == -1)
				return (ft_putstr_fd("minishell : malloc error", 2), -1);
		}
		prv_type = t->type;
		t = t->next;
	}
	if (handle_expanded_var(token, WORD, 0) == -1)
		return (-1);
	return (0);
}
