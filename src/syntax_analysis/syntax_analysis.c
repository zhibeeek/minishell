/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analysis.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zzhyrgal <zzhyrgal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 08:35:11 by zzhyrgal          #+#    #+#             */
/*   Updated: 2025/11/16 16:35:01 by zzhyrgal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_ast	*parse_pipeline(t_token **tokens, t_heredoc **heredoc_list)
{
	t_ast	*right;
	t_ast	*left;
	t_ast	*pipe_node;

	if (!tokens || !*tokens)
		return (NULL);
	left = parse_command(tokens, heredoc_list);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		right = parse_command(tokens, heredoc_list);
		if (!right)
		{
			printf("syntax error");
			return (NULL);
		}
		pipe_node = create_pipe_node(NODE_PIPE, left, right);
		left = pipe_node;
	}
	return (left);
}

t_ast	*syntax_analysis(t_token **token_list, t_heredoc *heredocs)
{
	t_ast		*ast_tree;
	t_heredoc	*heredoc_list;

	if (!token_list)
		return (NULL);
	if (!syntax_check(token_list))
		return (NULL);
	heredoc_list = heredocs;
	ast_tree = parse_pipeline(token_list, &heredoc_list);
	if (heredoc_list)
	{
		printf("Error: heredoc count mismatch (extra heredocs not consumed)\n");
		free_heredoc_list(heredoc_list);
		if (ast_tree)
			free_ast(ast_tree);
		return (NULL);
	}
	return (ast_tree);
}
