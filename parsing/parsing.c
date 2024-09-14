/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:54:16 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/09/14 16:11:00 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

char	*ft_add_char(char *s, unsigned int index, char to_add)
{
	unsigned int	j;
	int				i;
	char			*str;

	i = 0;
	j = 0;
	if (ft_strlen(s) < index || !s)
		return (NULL);
	str = malloc(sizeof(char) * (ft_strlen(s) + 2));
	if (!str)
		return ((write(2, "Error\nadd_char can't alloc\n", 28)), free(s), NULL);
	while (s[i] != '\0')
	{
		if (j != index)
		{
			str[j] = s[i];
			i++;
		}
		else
			str[j] = to_add;
		j++;
	}
	str[j] = '\0';
	free(s);
	return (str);
}

char	*ft_handl_appand_herdoc(char *line, int i)
{
	int	len;

	len = ft_strlen(line);
	if (i > 0 && i + 1 < len)
	{
		if ((line[i - 1] != ' ' && line[i - 1] != '\t')
			&& (line[i + 2] != ' ' && line[i + 2] != '\t'))
		{
			line = ft_add_char(line, i, ' ');
			line = ft_add_char(line, i + 3, ' ');
		}
		else if (line[i - 1] != ' ' && line[i - 1] != '\t')
		{
			line = ft_add_char(line, i, ' ');
		}
		else if (line[i + 2] != ' ' && line[i + 2] != '\t')
		{
			line = ft_add_char(line, i + 2, ' ');
		}
	}
	else if (i == 0 && (line[i + 2] != ' ' && line[i + 2] != '\t'))
		return (ft_add_char(line, i + 2, ' '));
	return (line);
}

char	*ft_handl_spc_opr(char *line, int i)
{
	int	len;

	len = ft_strlen(line);
	if (i > 0 && i + 1 < len)
	{
		if ((line[i - 1] != ' ' || line[i - 1] != '\t')
			&& (line[i + 1] != ' ' || line[i + 1] != '\t'))
		{
			line = ft_add_char(line, i, ' ');
			line = ft_add_char(line, i + 2, ' ');
		}
		else if (line[i - 1] != ' ' && line[i - 1] != '\t')
		{
			line = ft_add_char(line, i, ' ');
		}
		else if (line[i + 1] != ' ' && line[i + 1] != '\t')
		{
			line = ft_add_char(line, i + 1, ' ');
		}
	}
	else if (i == 0 && (line[i + 1] != ' ' && line[i + 1] != '\t'))
		return (ft_add_char(line, i + 1, ' '));
	return (line);
}

char	*ft_add_space(char *line, size_t i)
{
	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'')
			i = ft_skipe_qoute(line, i) - 1;
		else if ((i + 2 < ft_strlen(line))
			&& ((line[i] == '>' && line[i + 1] == '>')
				|| (line[i] == '<' && line[i + 1] == '<')))
		{
			line = ft_handl_appand_herdoc(line, i);
			if (!line)
				return (NULL);
			i += 2;
		}
		else if ((i + 1 < ft_strlen(line)) && is_spc_opr(line, i) == 1)
		{
			line = ft_handl_spc_opr(line, i);
			if (!line)
				return (NULL);
			i++;
		}
		i++;
	}
	return (line);
}

int	parsing(char *line, t_token	**token, t_env *env, int exit_status)
{
	t_token	*tmp;

	line = ft_add_space(line, 0);
	if (ft_check_qoutes(line) == -1)
		return (-1);
	line = ft_compress_spaces(line);
	if (get_token(token, line, 1) == -1)
		return (-1);
	if (*token == NULL)
		return (0);
	if (expanding(token, env, exit_status, WORD) == -1)
		return (-1);
	tmp = *token;
	while (tmp)
	{
		if (tmp->content[ft_skipe_spaces(tmp->content, 0)] == '\0')
			tmp->is_empty = true;
		if (tmp->qout_rm == true && tmp->type != HER_DOC)
			if (ft_remove_quotes(tmp, 0, 0) == -1)
				return (-1);
		if (tmp->type == HER_DOC && tmp->next != NULL)
			tmp = tmp->next;
		tmp = tmp->next;
	}
	return (free(line), ft_check_error(*token));
}
