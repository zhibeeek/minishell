/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adavitas <adavitas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 18:14:28 by adavitas          #+#    #+#             */
/*   Updated: 2025/11/17 19:02:02 by adavitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_cmd_node(t_ast *node, t_env *env, int fd_in, int fd_out)
{
	pid_t	pid;
	int		status;

	if (fd_in == STDIN_FILENO && fd_out == STDOUT_FILENO
		&& is_builtin(node->argv[0]))
		return (execute_builtin_with_redirs_ast(node, env, 0));
	pid = fork();
	if (pid == 0)
		execute_ast_child(node, env, fd_in, fd_out);
	if (fd_in != STDIN_FILENO)
		close(fd_in);
	if (fd_out != STDOUT_FILENO)
		close(fd_out);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

int	execute_ast_recursive(t_ast *node, t_env *env, int fd_in, int fd_out)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
	{
		if (!node->argv || !node->argv[0] || !node->argv[0][0])
			return (0);
		return (execute_cmd_node(node, env, fd_in, fd_out));
	}
	else if (node->type == NODE_PIPE)
		return (execute_pipe_node(node, env, fd_in, fd_out));
	return (0);
}

int	execute_ast(t_ast *ast, t_env *env)
{
	if (!ast)
		return (0);
	return (execute_ast_recursive(ast, env, STDIN_FILENO, STDOUT_FILENO));
}
