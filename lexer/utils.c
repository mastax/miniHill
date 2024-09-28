/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:45:04 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/09/27 22:04:20 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

char	*get_var_from_env(char *var, size_t var_len, t_env *env)
{
	int	i;

	i = 0;
	while (env->env_vars[i] != NULL)
	{
		if (ft_strncmp(env->env_vars[i], var, var_len) == 0
			&& env->env_vars[i][var_len] == '=')
			return (&(env->env_vars[i][var_len + 1]));
		i++;
	}
	return (NULL);
}

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*str;
	size_t	slen;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (start < slen && s[start + i] != '\0' && i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

int	is_spc_opr(char *s, int i)
{
	if (s[i] == '|' || s[i] == '>' || s[i] == '<')
		return (1);
	return (0);
}

int	ft_skipe_spaces(char *line, int i)
{
	while ((line[i] == ' ' || line[i] == '\t') && line[i] != '\0')
		i++;
	return (i);
}

int	ft_skipe_qoute(char *s, int i)
{
	char	quote;

	quote = s[i];
	if (((s[i] == '"' && s[i + 1] == '"') || (s[i] == '\'' && s[i + 1] == '\''))
		&& s[i + 2] == '\0')
		return (i + 2);
	if (quote)
	{
		i++;
		while (s[i] != '\0' && s[i] != quote)
			i++;
		if (s[i] == quote)
			i++;
	}
	while (s[i] != '\0')
	{
		if (s[i] == ' ' || s[i] == '\t' || s[i] == '|' || s[i] == '>'
			|| s[i] == '<')
			break ;
		if (s[i] == '"' || s[i] == '\'')
			i = ft_skipe_qoute(s, i);
		else
			i++;
	}
	return (i);
}
