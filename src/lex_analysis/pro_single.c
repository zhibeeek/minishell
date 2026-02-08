/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pro_single.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zzhyrgal <zzhyrgal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 21:35:59 by zzhyrgal          #+#    #+#             */
/*   Updated: 2025/11/20 23:39:24 by zzhyrgal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

t_quote_token	*create_quote_node(t_quote_type qtype, int len)
{
	t_quote_token	*q;

	q = malloc(sizeof(t_quote_token));
	if (!q)
		return (NULL);
	q->qtype = qtype;
	q->length = len;
	q->next = NULL;
	return (q);
}

static int	append_content(t_token *token, char *content)
{
	char	*new_value;

	new_value = ft_strjoin(token->value, content);
	free(token->value);
	free(content);
	if (!new_value)
		return (0);
	token->value = new_value;
	return (1);
}

int	find_closing_quote(char *input, int start, char quote_char)
{
	int	end;

	end = start + 1;
	while (input[end] && input[end] != quote_char)
		end++;
	if (!input[end])
		return (-1);
	return (end);
}

int	handle_quote_content(t_token *token, char *content)
{
	if (token->value)
	{
		if (!append_content(token, content))
			return (0);
	}
	else
		token->value = content;
	return (1);
}

int	process_single_quote(t_token *token, char *input, int pos)
{
	int				end;
	char			*content;
	t_quote_token	*quote_node;

	if (input[pos] != '\'')
		return (0);
	end = find_closing_quote(input, pos, '\'');
	if (end < 0)
	{
		ft_putstr_fd("minishell: unclosed quote\n", 2);
		return (-1);
	}
	content = ft_substr(input, pos + 1, end - pos - 1);
	if (!content)
		return (0);
	quote_node = create_quote_node(SINGLE_QUOTE, end - pos - 1);
	if (!quote_node)
		return (0);
	if (!handle_quote_content(token, content))
		return (0);
	append_quote_to_chain(&token->quote, quote_node);
	return (end - pos + 1);
}
