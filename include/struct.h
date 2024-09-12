/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 20:34:10 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/12 20:34:12 by elel-bah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>
# include <stdbool.h>
# include <signal.h>

typedef struct x_arg
{
	char			**arg;
	char			**red;
	int				*heredoc_fds;
	struct x_arg	*next;
}	t_arg;

typedef enum s_type
{
	WORD,
	PIPE,
	RED_IN,
	RED_OUT,
	HER_DOC,
	APPEND,
}	t_type;

typedef struct s_tokenz
{
	char			*content;
	int				type;
	bool			qout_rm;
	bool			is_empty;
	int				j;
	struct s_tokenz	*next;
}	t_token;

// Add this to your header file (mini_shell.h)
typedef struct s_env
{
	char	**env_vars;
	int		count;
}	t_env;

typedef struct s_io
{
	int	original_stdin;
	int	original_stdout;
}	t_io;

typedef struct s_redir
{
	int	fd;
	int	original_fd;
	int	is_output;
	int	type;
}	t_redir;

typedef struct s_pipeline_state //for pip
{
	int		prev_input;
	int		pipe_fds[2];
	int		*pids;
	int		num_commands;
	t_env	*env;
}	t_pipeline_state;

typedef struct s_delimiter_info
{
	char			**delimiter;
	char			**processed_delimiter;
	t_fd_tracker	*tracker;
}	t_delimiter_info;

typedef struct child_setup_params
{
	int	cmd_index;
	int	pipe_count;
	int	(*pipe_fds)[2];
	int	*heredoc_fds;
	int	heredoc_count;
}	t_child_setup_params;

#endif
