/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zzhyrgal <zzhyrgal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 19:03:58 by zzhyrgal          #+#    #+#             */
/*   Updated: 2025/11/18 07:33:16 by zzhyrgal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_heredoc	*pop_heredoc(t_heredoc **list)
{
	t_heredoc	*result;

	if (!list || !*list)
		return (NULL);
	result = *list;
	*list = (*list)->next;
	result->next = NULL;
	return (result);
}

static void	handle_token_loop(t_token **tokens, t_heredoc **heredoc_list,
		t_heredoc **heredoc_cmd_list, t_redir **redir_list)
{
	t_node_type	redir_type;

	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if ((*tokens)->type == TOKEN_WORD)
			*tokens = (*tokens)->next;
		else if (is_redirection((*tokens)->type))
		{
			redir_type = detect_redir_type(*tokens);
			if (redir_type == NODE_HEREDOC)
			{
				*tokens = (*tokens)->next->next;
				append_heredoc(heredoc_cmd_list, pop_heredoc(heredoc_list));
			}
			else
				append_redir(redir_list, create_redir(redir_type, tokens));
		}
		else
			break ;
	}
}

t_ast	*parse_command(t_token **tokens, t_heredoc **heredoc_list)
{
	t_ast		*command_node;
	t_redir		*redir_list;
	t_heredoc	*heredoc_cmd_list;
	t_token		*word_start;

	redir_list = NULL;
	heredoc_cmd_list = NULL;
	if (!*tokens || (*tokens)->type == TOKEN_PIPE)
		return (NULL);
	command_node = create_ast_node(NODE_COMMAND);
	if (!command_node)
		return (NULL);
	word_start = *tokens;
	handle_token_loop(tokens, heredoc_list, &heredoc_cmd_list, &redir_list);
	command_node->quote_chains = store_quote_types(word_start);
	command_node->argv = store_avs(&word_start);
	command_node->redirections = redir_list;
	command_node->heredocs = heredoc_cmd_list;
	return (command_node);
}
