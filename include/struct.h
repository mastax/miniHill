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
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/stat.h>

# define MAX_PIPES 1024
# define MAX_FD 1024  // Adjust this based on your system's limits
typedef struct s_fd_tracker
{
	int	fd_table[MAX_FD];
	int	qout;
}	t_fd_tracker;

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
	char            *befor_exp;
	int				type;
	bool			qout_rm;
	bool			is_empty;
	int				j;
	struct s_tokenz	*next;
}	t_token;

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

//=-=--=-=-=-=-=-

typedef struct s_setup_context
{
	t_arg	*cmd;
	t_env	*env;
	t_io	*io;
	// int		**pipe_fds;
	int		(*pipe_fds)[2];
	int		*command_count;
	int		*pipe_count;
}	t_setup_context;

typedef struct s_command_context
{
	t_arg			*cmd;
	t_env			*env;
	int				*exit_status;
	t_fd_tracker	*fd_tracker;
}	t_command_context;

typedef struct s_command_context1
{
	t_arg	*cmd;
	t_env	*env;
	int		*exit_status;
	t_io	*io;
	int		(*pipe_fds)[2];
	int		command_count;
	int		pipe_count;
}	t_command_context1;

typedef struct s_process_data
{
	t_command_context1	*ctx;
	pid_t				*pids;
	int					child_count;
	int					cmd_index;
}	t_process_data;

typedef struct s_execution_data
{
	t_io	io;
	int		command_count;
	int		pipe_count;
	int		pipe_fds[MAX_PIPES][2];
}	t_execution_data;

#endif
