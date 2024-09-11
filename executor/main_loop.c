/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:53:33 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/11 20:53:34 by elel-bah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

static int	handle_input(char *input, t_env *env)
{
	(void)env;
	if (!input)
	{
		ft_putstr_fd("\nExiting shell.\n", 1);
		return (1);
	}
	if (*input)
		add_history(input);
	return (0);
}

static int	process_command(t_token *tokens, t_env *env)
{
	t_arg				*cmd;
	int					result;
	int					exit_status;
	t_fd_tracker		fd_tracker;
	t_command_context	context;

	exit_status = get_exit_status(-500);
	fd_tracker = {0};
	if (!tokens)
		return (0);
	cmd = ft_arg_new(2);
	if (!cmd)
		return (0);
	if (ft_convert_token_to_arg(tokens, cmd, 0) == 0)
	{
		context.cmd = cmd;
		context.env = env;
		context.exit_status = &exit_status;
		context.fd_tracker = &fd_tracker;
		result = execute_command(&context);
		free_command(cmd);
		return (result);
	}
	free_command(cmd);
	return (0);
}

int	handle_parse_result(int parse_result, t_token **tokens, int *exit_status)
{
	if (parse_result == -1)
	{
		free_tokens(*tokens);
		get_exit_status(1);
	}
	else if (parse_result == 258)
	{
		free_tokens(*tokens);
		get_exit_status(258);
	}
	*exit_status = get_exit_status(-500);
	return (parse_result);
}

int	process_input(char *input, t_env *env, t_token **tokens, int *exit_status)
{
	int	parse_result;

	if (handle_input(input, env))
		return (-1);
	parse_result = parsing(input, tokens, env, *exit_status);
	if (parse_result == 0)
	{
		if (process_command(*tokens, env) == -1)
		{
			free_tokens(*tokens);
			return (-1);
		}
		free_tokens(*tokens);
		*tokens = NULL;
	}
	return (handle_parse_result(parse_result, tokens, exit_status));
}

int	main_shell_loop(t_env *env, t_fd_tracker *fd_tracker)
{
	char	*input;
	t_token	*tokens;
	int		exit_status;

	exit_status = 0;
	sig_init();
	while (1)
	{
		initialize_loop_iteration(&tokens);
		if (get_sigint(-500))
		{
			get_sigint(0);
			exit_status = get_exit_status(-500);
		}
		input = readline(GREEN"minishell> "RESET);
		if (input == NULL)
		{
			write(STDERR_FILENO, "exit\n", 6);
			break ;
		}
		if (process_input(input, env, &tokens, &exit_status) == -1)
			close_all_fds(fd_tracker);
		// close_all_fds(fd_tracker);
	}
	close_all_fds(fd_tracker);
	return (exit_status);
}
