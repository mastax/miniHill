/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:37:25 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/09/26 20:05:12 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

int	check_if_more_to_expand(char *s, int i)
{
	i++;
	while (s[i] != '\0')
	{
		if (s[i] == '$' && (is_valid_var(s[i + 1]) == 1))
			return (1);
		i++;
	}
	return (0);
}

int	handel_check_can_expand(t_token *t, int case_indx)
{
	if (case_indx == 1 || case_indx == 2)
	{
		if (had_qout(t->content) == 1)
			t->qout_rm = false;
		if (check_if_more_to_expand(t->content, t->i) == 0)
			t->had_exp = true;
		return (1);
	}
	else if (case_indx == 3)
	{
		if (handle_spp_case(t->content) == 1)
			return (1);
		ambiguous_error(t->content);
		return (0);
	}
	return (0);
}

int	check_can_expand(char *var_name, t_env *env, t_type prv_type, t_token *t)
{
	if ((prv_type == WORD || prv_type == PIPE) && t->had_exp == false)
	{
		return (handel_check_can_expand(t, 1));
	}
	else if ((get_var_from_env(var_name, t->j, env) != NULL)
		&& t->had_exp == false
		&& (prv_type == APPEND || prv_type == RED_IN || prv_type == RED_OUT))
	{
		return (handel_check_can_expand(t, 2));
	}
	else if ((get_var_from_env(var_name, t->j, env) == NULL)
		&& (prv_type == APPEND || prv_type == RED_IN || prv_type == RED_OUT))
	{
		return (handel_check_can_expand(t, 3));
	}
	return (2);
}

static int	handle_variable_expansion(t_token *t, t_env *env, t_type prv_type
, int *i)
{
	char	*var_name;
	int		check_result;

	t->j = ft_name_len(t->content, *i + 1);
	var_name = ft_substr(t->content, *i + 1, t->j);
	if (!var_name)
		return (ft_putstr_fd("minishell : malloc error", 2), -1);
	t->i = *i;
	check_result = check_can_expand(var_name, env, prv_type, t);
	if (check_result == 0)
		return (free(var_name), -1);
	else if (check_result == 1)
	{
		if (ft_var_update(*i, &(t->content), get_var_from_env(var_name
					, t->j, env), &t->content[*i + t->j + 1]) == -1)
			return (free(var_name), ft_putstr_fd("minishell : malloc error", 2)
				, -1);
	}
	else if (check_result == 2)
		return (1);
	if (get_var_from_env(var_name, t->j, env) != NULL)
		*i = *i + 1;
	free(var_name);
	return (0);
}

int	ft_expand_variable(t_token *t, t_env *env, t_type prv_type, int i)
{
	int	result;

	while (t->content[i] != '\0')
	{
		if (t->content[i] == '\'' && check_induble(t->content, i) == 0)
			i = expanding_helper(t, i);
		else if (t->content[i] == '$' && (is_valid_var(t->content[i + 1]) == 1))
		{
			result = handle_variable_expansion(t, env, prv_type, &i);
			if (result == -1)
				return (-1);
			else if (result == 1)
				i++;
		}
		else if ((t->content[i] == '$' && (is_valid_var(t->content[i + 1])
					!= 1)) || (t->content[i] != '$' && t->content[i] != '\0'))
			i++;
	}
	return (0);
}
