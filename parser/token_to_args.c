/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 20:22:10 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/09/12 20:25:56 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

int	ft_handle_convert_args(t_token *token, t_arg *args)
{
	int	i;

	if (args->arg == NULL)
	{
		i = count_args_red(token, 1, 0);
		args->arg = malloc(sizeof(char *) * (i + 1));
		if (args->arg == NULL)
			return (ft_putstr_fd("Error: can't malloc for new_arg\n", 2), -1);
		if (token->is_empty == true)
			args->arg = ft_handl_args(NULL, args->arg, 1);
		else
			args->arg = ft_handl_args(token->content, args->arg, 1);
	}
	else
		args->arg = ft_handl_args(token->content, args->arg, 2);
	return (0);
}

int	ft_handle_convert_red(t_token *token, t_arg *args)
{
	int	i;

	if (args->red == NULL)
	{
		i = count_args_red(token, 2, 0);
		args->red = malloc(sizeof(char *) * (i + 1));
		if (args->red == NULL)
			return (ft_putstr_fd("Error: can't malloc for new_arg\n", 2), -1);
		args->red = ft_handl_args(token->content, args->red, 1);
		args->red = ft_handl_args(token->next->content, args->red, 2);
	}
	else
	{
		args->red = ft_handl_args(token->content, args->red, 2);
		args->red = ft_handl_args(token->next->content, args->red, 2);
	}
	return (0);
}

int	ft_handle_convert(t_token *token, t_arg *args, int type)
{
	if (type == 1)
		return (ft_handle_convert_args(token, args));
	else if (type == 2)
		return (ft_handle_convert_red(token, args));
	return (0);
}

int	ft_convert_token_to_arg(t_token *token, t_arg *args, int stat)
{
	t_arg	*tmp_arg;
	t_token	*tmp_token;

	tmp_arg = args;
	tmp_token = token;
	while (tmp_token)
	{
		if (tmp_token->type == WORD)
			stat = ft_handle_convert(tmp_token, tmp_arg, 1);
		else if (tmp_token->type == HER_DOC || tmp_token->type == APPEND
			|| tmp_token->type == RED_IN || tmp_token->type == RED_OUT)
		{
			stat = ft_handle_convert(tmp_token, tmp_arg, 2);
			tmp_token = tmp_token->next;
		}
		else if (tmp_token->type == PIPE)
		{
			ft_argadd_back(&args, ft_arg_new(2));
			tmp_arg = ft_arglast(args);
		}
		if (!tmp_arg || stat == -1)
			return (-1);
		tmp_token = tmp_token->next;
	}
	return (0);
}
