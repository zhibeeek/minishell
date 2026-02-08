/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zzhyrgal <zzhyrgal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 00:18:09 by zzhyrgal          #+#    #+#             */
/*   Updated: 2025/11/16 16:35:02 by zzhyrgal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_redirection(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (1);
	else if (type == TOKEN_REDIR_OUT)
		return (1);
	else if (type == TOKEN_APPEND)
		return (1);
	else if (type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

t_node_type	detect_redir_type(t_token *tokens)
{
	if (!tokens)
		return (NODE_INVALID);
	if ((tokens)->type == TOKEN_REDIR_IN)
		return (NODE_REDIR_IN);
	else if ((tokens)->type == TOKEN_REDIR_OUT)
		return (NODE_REDIR_OUT);
	else if ((tokens)->type == TOKEN_APPEND)
		return (NODE_APPEND);
	else if ((tokens)->type == TOKEN_HEREDOC)
		return (NODE_HEREDOC);
	return (NODE_INVALID);
}

void	append_redir(t_redir **redir_list, t_redir *new_redir)
{
	t_redir	*tmp;

	if (!new_redir || !redir_list)
		return ;
	new_redir->next = NULL;
	if (!*redir_list)
	{
		*redir_list = new_redir;
		return ;
	}
	tmp = *redir_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_redir;
}

t_quote_type	get_token_quote_type(t_token *token)
{
	if (!token || !token->quote)
		return (NO_QUOTE);
	return (token->quote->qtype);
}

void	append_heredoc(t_heredoc **heredoc_list, t_heredoc *new_heredoc)
{
	t_heredoc	*tmp;

	if (!new_heredoc || !heredoc_list)
		return ;
	new_heredoc->next = NULL;
	if (!*heredoc_list)
	{
		*heredoc_list = new_heredoc;
		return ;
	}
	tmp = *heredoc_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_heredoc;
}
