/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pro_double.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adavitas <adavitas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 23:27:18 by zzhyrgal          #+#    #+#             */
/*   Updated: 2025/11/20 22:49:02 by adavitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

void	append_quote_to_chain(t_quote_token **chain, t_quote_token *new_node)
{
	t_quote_token	*current;

	if (!*chain)
	{
		*chain = new_node;
		return ;
	}
	current = *chain;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

int	process_double_quote(t_token *token, char *input, int pos)
{
	int				end;
	char			*content;
	t_quote_token	*quote_node;

	if (input[pos] != '"')
		return (0);
	end = find_closing_quote(input, pos, '"');
	if (end < 0)
	{
		ft_putstr_fd("minishell: unclosed quote\n", 2);
		return (-1);
	}
	content = ft_substr(input, pos + 1, end - pos - 1);
	if (!content)
		return (0);
	quote_node = create_quote_node(DOUBLE_QUOTE, end - pos - 1);
	if (!quote_node)
		return (0);
	if (!handle_quote_content(token, content))
		return (0);
	append_quote_to_chain(&token->quote, quote_node);
	return (end - pos + 1);
}
