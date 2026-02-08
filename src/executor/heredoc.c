/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zzhyrgal <zzhyrgal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 18:14:56 by adavitas          #+#    #+#             */
/*   Updated: 2025/11/20 19:13:52 by zzhyrgal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	read_heredoc_lines(int fd, char *delimiter)
{
	char	*line;

	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (g_signal_received == SIGINT)
		{
			free(line);
			setup_signals();
			return (1);
		}
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	setup_signals();
	return (0);
}

static t_heredoc	*handle_heredoc_error(char *delimiter, char *temp_filename,
		t_heredoc *heredoc_list, int fd)
{
	if (fd >= 0)
	{
		close(fd);
		unlink(temp_filename);
	}
	free(delimiter);
	free(temp_filename);
	free_heredoc_list(heredoc_list);
	return (NULL);
}

static t_heredoc	*finalize_heredoc(char *delimiter, char *temp_filename,
		t_heredoc *heredoc_list)
{
	t_heredoc	*new_heredoc;

	new_heredoc = create_heredoc_node(delimiter, temp_filename);
	if (!new_heredoc)
		return (handle_heredoc_error(delimiter, temp_filename,
				heredoc_list, -1));
	append_heredoc_to_list(&heredoc_list, new_heredoc);
	return (heredoc_list);
}

static t_heredoc	*process_single_heredoc(char *line, int i,
		t_heredoc *heredoc_list, int heredoc_index)
{
	char		*delimiter;
	char		*temp_filename;
	int			fd;

	delimiter = extract_heredoc_delimiter(line, i);
	if (!delimiter || !*delimiter)
		return (heredoc_list);
	temp_filename = generate_temp_filename(heredoc_index);
	if (!temp_filename)
		return (free(delimiter), free_heredoc_list(heredoc_list), NULL);
	fd = open_heredoc_tmp(temp_filename);
	if (fd < 0)
		return (handle_heredoc_error(delimiter, temp_filename,
				heredoc_list, -1));
	if (read_heredoc_lines(fd, delimiter) != 0)
		return (handle_heredoc_error(delimiter, temp_filename,
				heredoc_list, fd));
	close(fd);
	return (finalize_heredoc(delimiter, temp_filename, heredoc_list));
}

t_heredoc	*process_heredoc_input(char *line)
{
	int			i;
	int			heredoc_index;
	t_heredoc	*heredoc_list;

	i = 0;
	heredoc_index = 0;
	heredoc_list = NULL;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
			i = skip_quoted_section(line, i);
		else if (line[i] == '<' && line[i + 1] == '<')
		{
			heredoc_list = process_single_heredoc(line, i,
					heredoc_list, heredoc_index);
			if (!heredoc_list && heredoc_index > 0)
				return (NULL);
			if (heredoc_list)
				heredoc_index++;
			i++;
		}
		else
			i++;
	}
	return (heredoc_list);
}
