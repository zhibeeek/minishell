/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_node_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zzhyrgal <zzhyrgal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 00:18:14 by zzhyrgal          #+#    #+#             */
/*   Updated: 2025/11/16 16:35:02 by zzhyrgal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_ast	*create_command_node(t_token **tokens)
{
	t_ast			*cmd_node;
	char			**arr;
	t_quote_token	**quote_chains;
	t_token			*saved_tokens;

	if (!tokens || !*tokens)
		return (NULL);
	cmd_node = create_ast_node(NODE_COMMAND);
	if (!cmd_node)
		return (NULL);
	saved_tokens = *tokens;
	quote_chains = store_quote_types(saved_tokens);
	arr = store_avs(tokens);
	if (!arr)
	{
		free(cmd_node);
		free(quote_chains);
		return (NULL);
	}
	cmd_node->argv = arr;
	cmd_node->quote_chains = quote_chains;
	return (cmd_node);
}

t_redir	*create_redir(t_node_type type, t_token **tokens)
{
	t_redir	*redir_node;

	if (!tokens || !*tokens || !(*tokens)->next)
		return (NULL);
	redir_node = malloc(sizeof(t_redir));
	if (!redir_node)
		return (NULL);
	redir_node->type = type;
	redir_node->filename = ft_strdup((*tokens)->next->value);
	if (!redir_node->filename)
	{
		free(redir_node);
		return (NULL);
	}
	redir_node->next = NULL;
	*tokens = (*tokens)->next->next;
	return (redir_node);
}

t_ast	*create_pipe_node(t_node_type type, t_ast *left, t_ast *right)
{
	t_ast	*pipe_node;

	pipe_node = create_ast_node(type);
	if (!pipe_node)
		return (NULL);
	pipe_node->type = type;
	pipe_node->argv = NULL;
	pipe_node->redirections = NULL;
	pipe_node->left = left;
	pipe_node->right = right;
	return (pipe_node);
}

t_ast	*create_ast_node(t_node_type type)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->argv = NULL;
	node->quote_chains = NULL;
	node->redirections = NULL;
	node->heredocs = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}
