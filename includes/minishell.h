/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adavitas <adavitas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 18:13:09 by adavitas          #+#    #+#             */
/*   Updated: 2025/11/22 12:19:09 by adavitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/ioctl.h>
# include <stdio.h>
# include <signal.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <ctype.h>
# include "libft.h"
# include "tokenizer.h"
# include "parser.h"

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}	t_env_var;

typedef struct s_env
{
	t_env_var	*vars;
	char		**envp_array;
}	t_env;

typedef struct s_expand_ctx
{
	t_env	*env;
	int		last_status;
}	t_expand_ctx;

extern int	g_signal_received;

int			execute_ast(t_ast *ast, t_env *env);
int			execute_ast_recursive(t_ast *node, t_env *env, int fd_in,
				int fd_out);
char		*get_command_path(char *command, t_env *env);
void		execute_command(char **args, t_env *env);
void		expand_ast(t_ast *ast, t_env *env, int last_status);
void		copy_non_empty(char **argv, char **new_argv,
				t_quote_token **quote_chains, t_quote_token **new_chains);
char		**compact_argv(char **argv, t_quote_token ***quote_chains_ptr);
char		*expand_segment(char *arg, int pos, t_quote_token *cur,
				t_expand_ctx *ctx);
char		*process_quote_chain(char *arg, t_quote_token *chain,
				t_expand_ctx *ctx);
void		setup_redirections(t_redir *redirs);
void		setup_heredocs(t_heredoc *heredocs);
int			open_infile(char *path);
int			open_outfile(char *path, int append);
void		handle_redir_in(t_redir *redir);
void		handle_redir_out(t_redir *redir, int append);
t_heredoc	*process_heredoc_input(char *line);
char		*generate_temp_filename(int index);
int			open_heredoc_tmp(char *filename);
t_heredoc	*create_heredoc_node(char *delimiter, char *temp_filename);
void		append_heredoc_to_list(t_heredoc **list, t_heredoc *new_node);
char		*extract_heredoc_delimiter(char *line, int pos);
int			skip_quoted_section(char *line, int i);

void		setup_child_io(int fd_in, int fd_out);
void		restore_fds(int stdin_backup, int stdout_backup);
void		execute_ast_child(t_ast *node, t_env *env, int fd_in, int fd_out);
int			execute_builtin_ast(t_ast *node, t_env *env, int last_status);
int			execute_builtin_with_redirs_ast(t_ast *node, t_env *env,
				int last_status);

pid_t		fork_left_child(t_ast *pipe_node, t_env *env, int fd_in,
				int pipefd[2]);
int			fork_right_child(t_ast *pipe_node, t_env *env, int pipefd[2],
				int fd_out);
int			execute_pipe_node(t_ast *pipe_node, t_env *env, int fd_in,
				int fd_out);

int			is_builtin(char *cmd);
int			execute_builtin(t_cmd *cmd, t_env *env, int last_status);
int			builtin_echo(char **args);
int			builtin_cd(char **args, t_env *env);
int			builtin_pwd(void);
int			builtin_export(char **args, t_env *env);
int			builtin_unset(char **args, t_env *env);
int			builtin_env(t_env *env);
int			builtin_exit(char **args, int last_status);
void		print_export_format(t_env *env);

t_env		*init_env(char **envp);
char		*get_env_value(t_env *env, char *key);
void		set_env_value(t_env *env, char *key, char *value);
void		unset_env_value(t_env *env, char *key);
char		**env_to_array(t_env *env);
void		free_env(t_env *env);
char		*expand_vars(char *str, t_env *env, int last_status);

void		setup_signals(void);
void		setup_child_signals(void);
void		setup_heredoc_signals(void);

void		free_cmds(t_cmd *cmds);

void		print_error(char *cmd, char *arg, char *msg);
void		free_array(char **array);
void		close_unused_fds(void);

#endif
