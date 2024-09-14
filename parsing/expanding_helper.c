/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 16:13:35 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/09/14 16:18:38 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

char	*ft_remove_char(char *s, unsigned int index)
{
	unsigned int	i;
	int				j;
	char			*str;

	i = 0;
	j = 0;
	if (ft_strlen(s) < index || !s)
		return (NULL);
	str = malloc(ft_strlen(s));
	if (!str)
		return (printf("Error :remove_char can't alloc\n"), free(s), NULL);
	while (s[i] != '\0')
	{
		if (i == index)
			i++;
		else
			str[j++] = s[i++];
	}
	str[j] = '\0';
	free(s);
	return (str);
}

int	expanding_helper(char *s, int i)
{
	int	j;

	j = i;
	if (s[i] == '\'')
	{
		i++;
		while (s[i] != '\0' && s[i] != '\'')
			i++;
		if (s[i] == '\'')
			i++;
		else if (s[i] == '\0')
		{
			j++;
			return (j);
		}
		return (i);
	}
	return (0);
}

int	ft_var_update(int i, char **var, char *to_update, char	*secend_part)
{
	char	*first_part;
	char	*update_var;

	first_part = NULL;
	if (i == 0 && !to_update)
		first_part = ft_strdup("");
	else if (i == 0 && to_update)
		first_part = ft_strdup(to_update);
	else if (i != 0 && !to_update)
		first_part = ft_strndup(*var, i);
	else if (i != 0 && to_update)
		first_part = ft_strjoin(ft_strndup(*var, i), to_update);
	if (!first_part)
		return (-1);
	update_var = ft_strjoin(first_part, secend_part);
	if (!update_var)
	{
		free(first_part);
		return (-1);
	}
	free(*var);
	*var = update_var;
	return (0);
}
