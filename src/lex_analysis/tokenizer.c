/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adavitas <adavitas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 18:55:16 by zzhyrgal          #+#    #+#             */
/*   Updated: 2025/11/22 12:17:14 by adavitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int	deliminator(char c)
{
	if (c == ' ' || c == '|' || c == '<' || c == '>' || c == '\t' || c == '\0')
		return (1);
	return (0);
}

int	tokenize(t_token **token_list, char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	if (str[i] == '\'' || str[i] == '"')
		i += handle_word(token_list, &str[i]);
	else if (str[i] == '\\' || str[i] == ';')
		return (0);
	else if (is_redirect(&str[i]))
		i += handle_redirect(token_list, &str[i]);
	else if (str[i] == '|')
		i += handle_pipe(token_list, &str[i]);
	else
		i += handle_word(token_list, &str[i]);
	return (i);
}

t_token	*tokenize_input(char *input)
{
	t_token	*token_list;
	size_t	consumed;

	token_list = NULL;
	if (!input)
		return (NULL);
	consumed = 0;
	while (*input)
	{
		while (*input && ft_isspace((unsigned char)*input))
			input++;
		if (!*input)
			break ;
		consumed = tokenize(&token_list, input);
		if (consumed == 0)
			break ;
		input += consumed;
	}
	return (token_list);
}
