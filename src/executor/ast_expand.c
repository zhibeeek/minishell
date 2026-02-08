/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adavitas <adavitas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 23:09:04 by adavitas          #+#    #+#             */
/*   Updated: 2025/11/17 18:55:10 by adavitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_single_arg(char **argv, t_quote_token ***quote_chains_ptr,
		int i, t_expand_ctx *ctx)
{
	char	*new;

	if (!*quote_chains_ptr || !(*quote_chains_ptr)[i])
	{
		new = expand_vars(argv[i], ctx->env, ctx->last_status);
		free(argv[i]);
		argv[i] = new;
	}
	else
	{
		new = process_quote_chain(argv[i], (*quote_chains_ptr)[i], ctx);
		if (new)
		{
			free(argv[i]);
			argv[i] = new;
		}
		free_quote_chain((*quote_chains_ptr)[i]);
		(*quote_chains_ptr)[i] = NULL;
	}
}

static void	expand_argv(char ***argv_ptr, t_quote_token ***quote_chains_ptr,
		t_env *env, int last_status)
{
	char			**argv;
	int				i;
	t_expand_ctx	ctx;

	if (!argv_ptr || !*argv_ptr)
		return ;
	argv = *argv_ptr;
	ctx.env = env;
	ctx.last_status = last_status;
	i = 0;
	while (argv[i])
	{
		expand_single_arg(argv, quote_chains_ptr, i, &ctx);
		i++;
	}
	*argv_ptr = compact_argv(argv, quote_chains_ptr);
}

static void	expand_redirections(t_redir *redir, t_env *env, int last_status)
{
	char	*expanded;

	while (redir)
	{
		if (redir->type != NODE_HEREDOC)
		{
			expanded = expand_vars(redir->filename, env, last_status);
			free(redir->filename);
			redir->filename = expanded;
		}
		redir = redir->next;
	}
}

void	expand_ast(t_ast *ast, t_env *env, int last_status)
{
	if (!ast)
		return ;
	if (ast->type == NODE_COMMAND)
	{
		expand_argv(&ast->argv, &ast->quote_chains, env, last_status);
		expand_redirections(ast->redirections, env, last_status);
	}
	else if (ast->type == NODE_PIPE)
	{
		expand_ast(ast->left, env, last_status);
		expand_ast(ast->right, env, last_status);
	}
}
