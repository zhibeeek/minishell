/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adavitas <adavitas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 18:28:10 by adavitas          #+#    #+#             */
/*   Updated: 2025/11/18 22:18:13 by adavitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*generate_temp_filename(int index)
{
	char	*num_str;
	char	*filename;

	num_str = ft_itoa(index);
	if (!num_str)
		return (NULL);
	filename = ft_strjoin(".heredoc_tmp_", num_str);
	free(num_str);
	return (filename);
}

int	open_heredoc_tmp(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		print_error(filename, NULL, strerror(errno));
	return (fd);
}

t_heredoc	*create_heredoc_node(char *delimiter, char *temp_filename)
{
	t_heredoc	*node;

	node = malloc(sizeof(t_heredoc));
	if (!node)
		return (NULL);
	node->delimiter = delimiter;
	node->temp_filename = temp_filename;
	node->next = NULL;
	return (node);
}

void	append_heredoc_to_list(t_heredoc **list, t_heredoc *new_node)
{
	t_heredoc	*current;

	if (!*list)
	{
		*list = new_node;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

void	free_heredoc_list(t_heredoc *heredocs)
{
	t_heredoc	*current;
	t_heredoc	*next;

	current = heredocs;
	while (current)
	{
		next = current->next;
		if (current->temp_filename)
		{
			unlink(current->temp_filename);
			free(current->temp_filename);
		}
		if (current->delimiter)
			free(current->delimiter);
		free(current);
		current = next;
	}
}
