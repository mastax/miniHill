/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:53:21 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/09/14 16:18:30 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

char	*ft_handle_quotes(char *s, int *i)
{
	int	j;

	j = *i;
	if (s[*i] == '"')
	{
		*i = *i + 1;
		while (s[*i] != '"' && s[*i] != '\0')
			*i = *i + 1;
	}
	else if (s[*i] == '\'')
	{
		*i = *i + 1;
		while (s[*i] != '\'' && s[*i] != '\0')
			*i = *i + 1;
	}
	if ((s[j] == '"' && s[*i] == '"') || (s[j] == '\''
			&& s[*i] == '\''))
	{
		s = ft_remove_char(s, j);
		s = ft_remove_char(s, *i - 1);
		*i = *i - 1;
		return (s);
	}
	else
		return (NULL);
}

char	*ft_remove_quotes1(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		i = ft_skipe_spaces(s, i);
		if (s[i] == '"' || s[i] == '\'')
		{
			if ((s[i] == '"' && s[i + 1] == '"') || (s[i] == '\''
					&& s[i + 1] == '\''))
			{
				s = ft_remove_char(s, i);
				s = ft_remove_char(s, i);
			}
			else
				s = ft_handle_quotes(s, &i);
		}
		else
		{
			while (s[i] != ' ' && s[i] != '\t' && s[i] != '\0'
				&& s[i] != '"' && s[i] != '\'')
				i++;
		}
	}
	return (s);
}

int	get_char_index(char *s, int index, char c)
{
	if (index == -1)
		index = 0;
	while (s[index] != '\0')
	{
		if (s[index] == c)
			return (index);
		index++;
	}
	return (-1);
}

static int	handle_quote(t_token *t, int *st1, int *st2, char qout)
{
	int	end1;
	int	end2;

	*st2 = get_char_index(t->befor_exp, *st2, qout);
	if (*st2 == -1)
		return (1);
	end1 = get_char_index(t->content, *st1 + 1, qout);
	end2 = get_char_index(t->befor_exp, *st2 + 1, qout);
	if (end1 == -1 || end2 == -1)
		return (0);
	t->content = ft_remove_char(t->content, *st1);
	t->content = ft_remove_char(t->content, end1 - 1);
	t->befor_exp = ft_remove_char(t->befor_exp, *st2);
	t->befor_exp = ft_remove_char(t->befor_exp, end2 - 1);
	if (!t->befor_exp || !t->content)
	{
		ft_putstr_fd("minishell: malloc error\n", 2);
		return (-1);
	}
	*st1 = end1 - 1;
	*st2 = end2 - 1;
	return (2);
}

int	ft_remove_quotes(t_token *t, int st1, int st2)
{
	int	result;

	while (t->content[st1] != '\0')
	{
		if (t->content[st1] == '\'' || t->content[st1] == '"')
		{
			result = handle_quote(t, &st1, &st2, t->content[st1]);
			if (result == 0 || result == -1)
				return (result);
			if (result == 1)
			{
				st1++;
				continue ;
			}
		}
		else
			st1++;
	}
	return (0);
}
