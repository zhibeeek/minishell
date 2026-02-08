/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zzhyrgal <zzhyrgal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 12:22:39 by zzhyrgal          #+#    #+#             */
/*   Updated: 2025/11/15 00:09:21 by zzhyrgal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_quote_token	*duplicate_quote_chain(t_quote_token *chain)
{
	t_quote_token	*new_chain;
	t_quote_token	*current;
	t_quote_token	*new_node;
	t_quote_token	*last;

	if (!chain)
		return (NULL);
	new_chain = NULL;
	last = NULL;
	current = chain;
	while (current)
	{
		new_node = create_quote_node(current->qtype, current->length);
		if (!new_node)
			return (NULL);
		if (!new_chain)
			new_chain = new_node;
		else
			last->next = new_node;
		last = new_node;
		current = current->next;
	}
	return (new_chain);
}

t_quote_token	**store_quote_types(t_token *tokens)
{
	int				count;
	t_quote_token	**chains;
	int				i;

	count = size_words(tokens);
	chains = malloc(sizeof(t_quote_token *) * (count + 1));
	if (!chains)
		return (NULL);
	i = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (tokens->type == TOKEN_WORD)
			chains[i++] = duplicate_quote_chain(tokens->quote);
		else if (is_redirection(tokens->type))
		{
			tokens = tokens->next;
			if (!tokens)
				break ;
		}
		tokens = tokens->next;
	}
	chains[i] = NULL;
	return (chains);
}
