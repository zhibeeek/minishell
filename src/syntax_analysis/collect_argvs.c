/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_argvs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zzhyrgal <zzhyrgal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 09:10:37 by zzhyrgal          #+#    #+#             */
/*   Updated: 2025/11/13 15:27:53 by zzhyrgal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	size_words(t_token *tokens)
{
	t_token	*tmp;
	int		count;

	tmp = tokens;
	count = 0;
	while (tmp && tmp->type != TOKEN_PIPE)
	{
		if (tmp->type == TOKEN_WORD)
			count++;
		else if (is_redirection(tmp->type))
		{
			tmp = tmp->next;
			if (!tmp)
				break ;
		}
		tmp = tmp->next;
	}
	return (count);
}

static void	collect_all_words(t_token **tokens, char **av)
{
	int	i;

	i = 0;
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			av[i++] = ft_strdup((*tokens)->value);
			*tokens = (*tokens)->next;
		}
		else if (is_redirection((*tokens)->type))
		{
			*tokens = (*tokens)->next;
			if (*tokens)
				*tokens = (*tokens)->next;
		}
		else
			*tokens = (*tokens)->next;
	}
	av[i] = NULL;
}

char	**store_avs(t_token **tokens)
{
	int		count;
	char	**av;

	if (!tokens || !*tokens)
		return (NULL);
	count = size_words(*tokens);
	av = malloc(sizeof(char *) * (count + 1));
	if (!av)
		return (NULL);
	collect_all_words(tokens, av);
	return (av);
}
