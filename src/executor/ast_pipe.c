/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zzhyrgal <zzhyrgal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 18:14:45 by adavitas          #+#    #+#             */
/*   Updated: 2025/11/21 01:00:38 by zzhyrgal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	fork_left_child(t_ast *pipe_node, t_env *env, int fd_in, int pipefd[2])
{
	pid_t	pid_left;
	int		status;

	pid_left = fork();
	if (pid_left == 0)
	{
		close(pipefd[0]);
		if (pipefd[1] != STDOUT_FILENO)
		{
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
		}
		if (fd_in != STDIN_FILENO)
		{
			dup2(fd_in, STDIN_FILENO);
			close(fd_in);
		}
		close_unused_fds();
		status = execute_ast_recursive(pipe_node->left, env, STDIN_FILENO,
				STDOUT_FILENO);
		exit(status);
	}
	close(pipefd[1]);
	return (pid_left);
}

static void	right_child_process(t_ast *pipe_node, t_env *env, int pipefd[2],
		int fd_out)
{
	int	status;

	close(pipefd[1]);
	if (pipefd[0] != STDIN_FILENO)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
	}
	if (fd_out != STDOUT_FILENO)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	close_unused_fds();
	status = execute_ast_recursive(pipe_node->right, env, STDIN_FILENO,
			STDOUT_FILENO);
	exit(status);
}

int	fork_right_child(t_ast *pipe_node, t_env *env, int pipefd[2], int fd_out)
{
	pid_t	pid_right;
	int		status;

	pid_right = fork();
	if (pid_right == 0)
		right_child_process(pipe_node, env, pipefd, fd_out);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid_right, &status, 0);
	return (status);
}

int	execute_pipe_node(t_ast *pipe_node, t_env *env, int fd_in, int fd_out)
{
	int		pipefd[2];
	int		status;
	pid_t	pid_left;

	if (pipe(pipefd) == -1)
	{
		print_error("pipe", NULL, strerror(errno));
		return (1);
	}
	pid_left = fork_left_child(pipe_node, env, fd_in, pipefd);
	status = fork_right_child(pipe_node, env, pipefd, fd_out);
	if (fd_in != STDIN_FILENO)
		close(fd_in);
	waitpid(pid_left, NULL, 0);
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
