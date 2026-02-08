/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zzhyrgal <zzhyrgal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 18:14:25 by adavitas          #+#    #+#             */
/*   Updated: 2025/11/21 01:00:38 by zzhyrgal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin_ast(t_ast *node, t_env *env, int last_status)
{
	t_cmd	cmd_wrapper;

	cmd_wrapper.args = node->argv;
	cmd_wrapper.redirs = node->redirections;
	cmd_wrapper.next = NULL;
	return (execute_builtin(&cmd_wrapper, env, last_status));
}

int	execute_builtin_with_redirs_ast(t_ast *node, t_env *env, int last_status)
{
	int		stdin_backup;
	int		stdout_backup;
	int		status;
	t_cmd	cmd_wrapper;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	setup_heredocs(node->heredocs);
	setup_redirections(node->redirections);
	cmd_wrapper.args = node->argv;
	cmd_wrapper.redirs = node->redirections;
	cmd_wrapper.next = NULL;
	status = execute_builtin(&cmd_wrapper, env, last_status);
	fflush(stdout);
	restore_fds(stdin_backup, stdout_backup);
	return (status);
}

void	execute_ast_child(t_ast *node, t_env *env, int fd_in, int fd_out)
{
	setup_child_signals();
	setup_child_io(fd_in, fd_out);
	close_unused_fds();
	setup_heredocs(node->heredocs);
	setup_redirections(node->redirections);
	if (is_builtin(node->argv[0]))
		exit(execute_builtin_ast(node, env, 0));
	else
		execute_command(node->argv, env);
}
