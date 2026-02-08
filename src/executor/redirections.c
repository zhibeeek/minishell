/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zzhyrgal <zzhyrgal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 18:44:05 by adavitas          #+#    #+#             */
/*   Updated: 2025/11/16 16:35:02 by zzhyrgal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redir_in(t_redir *redir)
{
	int	fd;

	fd = open_infile(redir->filename);
	if (fd < 0)
		exit(1);
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	handle_redir_out(t_redir *redir, int append)
{
	int	fd;

	fd = open_outfile(redir->filename, append);
	if (fd < 0)
		exit(1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	setup_heredocs(t_heredoc *heredocs)
{
	t_heredoc	*current;
	int			fd;

	current = heredocs;
	while (current)
	{
		fd = open(current->temp_filename, O_RDONLY);
		if (fd < 0)
		{
			print_error("heredoc", current->temp_filename, strerror(errno));
			exit(1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
		unlink(current->temp_filename);
		current = current->next;
	}
}

void	setup_redirections(t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == NODE_REDIR_IN)
			handle_redir_in(current);
		else if (current->type == NODE_REDIR_OUT)
			handle_redir_out(current, 0);
		else if (current->type == NODE_APPEND)
			handle_redir_out(current, 1);
		current = current->next;
	}
}
