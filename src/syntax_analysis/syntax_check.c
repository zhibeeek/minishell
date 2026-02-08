/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adavitas <adavitas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 08:35:29 by zzhyrgal          #+#    #+#             */
/*   Updated: 2025/11/18 20:10:30 by adavitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	check_redir(t_token *current)
{
	if (!current->next)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
		ft_putstr_fd("`newline'\n", 2);
		return (1);
	}
	if (current->next->type != TOKEN_WORD)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(current->next->value, 2);
		ft_putstr_fd("'\n", 2);
		return (1);
	}
	return (0);
}

static int	check_pipe(t_token *current, t_token *prev)
{
	if (current->type == TOKEN_PIPE)
	{
		if (!current->next || current->next->type == TOKEN_PIPE
			|| current->next->type == TOKEN_UNKNOWN)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
			ft_putstr_fd("`|'\n", 2);
			return (1);
		}
		if (!prev || prev->type == TOKEN_PIPE || is_redirection(prev->type))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
			ft_putstr_fd("`|'\n", 2);
			return (1);
		}
	}
	return (0);
}

static int	check_tokens_loop(t_token *current)
{
	t_token	*prev;

	prev = NULL;
	while (current)
	{
		if (current->type == TOKEN_PIPE && check_pipe(current, prev))
			return (0);
		if (is_redirection(current->type) && check_redir(current))
			return (0);
		prev = current;
		current = current->next;
	}
	if (prev->type == TOKEN_PIPE || is_redirection(prev->type))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
		ft_putstr_fd("`newline'\n", 2);
		return (0);
	}
	return (1);
}

int	syntax_check(t_token **tokens)
{
	t_token	*current;

	if (tokens == NULL || *tokens == NULL)
		return (0);
	current = *tokens;
	if (current->type == TOKEN_PIPE || current->type == TOKEN_UNKNOWN)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (0);
	}
	return (check_tokens_loop(current));
}
