/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:56:29 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/17 16:45:35 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SHELL_H
# define MINI_SHELL_H

# include "./include/struct.h"
# include "./include/libft.h"

# define GREEN "\001\033[1;32m\002"
# define RESET "\001\033[0m\002"
# define MAX_COMMANDS 1024
# define STDIN_FILENO 0
# define STDOUT_FILENO 1
# define BUFFER_SIZE 1024

/* SIGNALS*/
void	sig_int(int code);
void	sig_quit(int code);
int		get_sigquit(int n);
int		get_pid(int n);
int		get_in_heredoc(int n);

// get_path
char	*ft_get_path(char *cmd, int *exit_status);

// Function prototypes
t_env	*create_env_v_i(int count);
t_env	*create_env(char **envp);
void	free_env(t_env *env);
char	*get_env_value(t_env *env, const char *key);
int		set_env_value(t_env *env, const char *key, const char *value);
int		unset_env_value(t_env *env, const char *key);
int		execute_builtin_p(t_arg *cmd, t_env *env, int *exit_status);
int		execute_builtin_ch(t_arg *cmd, t_env *env, int *exit_status);

int		execute_command(t_command_context *context);

int		change_to_home(t_env *env);
int		change_to_previous(t_env *env);
					/*PIP - HELPER FUNCTIONS*/

int		check_format(char *arg);
int		print_error(int error_code, char *content);
void	print_sorted_env(t_env *env);
int		parse_export_string(char *s, char **name, char **value, int *is_append);

					/*THE BUILTIN*/

int		ft_change_dir(char **av, t_env *env, int *exit_status);
int		ft_env(t_env *env);
int		ft_echo(char **av);
int		ft_exit(char **av, int *exit_status);
int		pwd(int fd);
int		ft_exports(t_env *env, char **args, int *exit_status);
int		ft_unsets(t_env *env, char **args, int *exit_status);

int		append_new_var(t_env *env, char *s);
					/*THE_EXECUTOR*/
int		main_shell_loop(t_env *env, t_fd_tracker *fd_tracker);
int		is_builtin(const char *cmd);
char	*find_command(char *cmd, char **envp);

void	restore_io(t_io *io);
void	save_original_io(t_io *io);
int		setup_pipes(int pipe_count, int pipe_fds[][2],
			t_fd_tracker *fd_tracker);
void	initialize_loop_iteration(t_token **tokens);
void	setup_child_process(t_child_setup_params *params);

void	execute_child_process_part1(t_command_context1 *ctx,
			t_arg *current_cmd);

void	configure_child_io(t_command_context1 *ctx, t_arg *current_cmd,
			int cmd_index);
void	cleanup_parent_process(int *heredoc_fds, int heredoc_count);
int		count_commands(t_arg *cmd);
int		wait_for_children(pid_t *pids, int command_count, int *exit_status);
void	free_tokens(t_token *tokens);
void	free_command(t_arg *cmd);

/*parsing*/

int		ft_var_update(int i, char **var, char *to_update, char	*secend_part);
int		is_valid_var(char c);
t_token	*ft_list_new(char *token, int z);
char	*ft_compress_spaces(char *s);
int		ft_check_qoutes(char	*line);
int		ft_skipe_qoute(char	*s, int i);
int		ft_name_len(char *var, int i);
int		ft_have_sp_tb(char *s);
int		ft_handl_spichel_cond(t_token **token, t_token *now,
			t_token *next_token, t_type	*prv_type);
int		get_token(t_token **token, char	*s, int z);
void	free_tokens(t_token *tokens);
int		ft_check_error(t_token *token);
void	ft_putstr_fd(char *str, int fd);

int		get_char_index(char *s, int index, char c);
char	*ft_remove_quotes1(char *s);
int		ft_remove_quotes(t_token *t, int st1, int st2);

int		count_args_red(t_token *token, int type, int i);
char	**ft_handl_args(char *content, char **arv, int type);

int		expanding(t_token **token, t_env *env, int exit_status,
			t_type prv_type);
int		expanding_helper(char *s, int i);
int		ft_skipe_spaces(char *s, int index);
char	*ft_remove_char(char *s, unsigned int index);
void	ft_lstadd_back(t_token **lst, t_token *new);
int		check_induble(char *s, int i);
int		is_spc_opr(char *s, int i);
int		parsing(char *line, t_token	**token, t_env *env, int exit_status);
t_arg	*ft_arg_new(int fd);
void	ft_argadd_back(t_arg **lst, t_arg *new);
t_arg	*ft_arglast(t_arg	*lst);
int		check_can_expand(char *var_name, t_env *env, t_type prv_type,
			t_token *t);
int		ft_convert_token_to_arg(t_token *token, t_arg *args, int stat);

/*Redirections functions*/
int		apply_redirections(char **red);

/*HERDOC*/

int		count_heredocs(char **red);
int		*handle_heredocs(char **red, int count, t_env *env,
			t_fd_tracker *tracker);

char	*read_line(void);
int		expand_variable(char **line, t_env *env);
int		write_to_pipe(int pipefd[2], char *line);

int		ft_expand_variable(t_token *t, t_env *env, t_type prv_type, int i);
char	*get_var_from_env(char *var, int var_len, t_env *env);
int		ft_var_update(int i, char **var, char *to_update, char	*secend_part);
int		ft_expand_herdoc_var(char **var, t_env *env, t_type prv_type, int i);

int		ft_expand_exit_status(char **var, int exit_status, int i);
int		get_exit_status(int n);

void	track_fd(t_fd_tracker *tracker, int fd);
void	untrack_fd(t_fd_tracker *tracker, int fd);
void	close_all_fds(t_fd_tracker *tracker);

/*CLEANUP*/

void	cleanup_heredoc_fds(t_arg *cmd, t_fd_tracker *fd_tracker);
void	cleanup_pipes(t_command_context1 *ctx);

/*heredoc helpers*/
void	child_process(int pipefd[2], const char *delimiter,
			t_env *env, t_fd_tracker *fd_tracker);
void	parent_process(int pipefd[2], pid_t pid, t_fd_tracker *fd_tracker);
int		check_if_qoutes(char *s);
int		setup_and_handle_heredocs(t_setup_context *ctx);
int		process_commands(t_command_context1 *ctx, pid_t *pids);
void	cleanup_and_return(int *heredoc_fds, int count, t_fd_tracker *tracker);

#endif
