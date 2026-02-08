/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_lists.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zzhyrgal <zzhyrgal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 19:18:07 by zzhyrgal          #+#    #+#             */
/*   Updated: 2025/11/14 15:18:07 by zzhyrgal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->value = ft_strdup(value);
	if (!new_node->value)
	{
		free(new_node);
		return (NULL);
	}
	new_node->type = type;
	new_node->quote = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	add_token_back(t_token **token_list, t_token *new_node)
{
	t_token	*current;

	if (!token_list || !new_node)
		return ;
	if (!*token_list)
	{
		*token_list = new_node;
		return ;
	}
	current = *token_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

void	free_quote_chain(t_quote_token *q)
{
	t_quote_token	*tmp;

	while (q)
	{
		tmp = q->next;
		free(q);
		q = tmp;
	}
}

void	free_tokens(t_token **head)
{
	t_token	*tmp;
	t_token	*current;

	if (!head || !*head)
		return ;
	current = *head;
	while (current)
	{
		tmp = current->next;
		if (current->value)
			free(current->value);
		if (current->quote)
			free_quote_chain(current->quote);
		free(current);
		current = tmp;
	}
	*head = NULL;
}
