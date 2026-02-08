/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_expand_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adavitas <adavitas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:40:12 by adavitas          #+#    #+#             */
/*   Updated: 2025/11/17 19:03:05 by adavitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_non_empty(char **argv, char **new_argv,
		t_quote_token **quote_chains, t_quote_token **new_chains)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (argv[i])
	{
		if (argv[i][0] != '\0')
		{
			new_argv[j] = argv[i];
			if (quote_chains)
				new_chains[j] = quote_chains[i];
			j++;
		}
		else
		{
			free(argv[i]);
			if (quote_chains && quote_chains[i])
				free_quote_chain(quote_chains[i]);
		}
		i++;
	}
	new_argv[j] = NULL;
	if (new_chains)
		new_chains[j] = NULL;
}

char	**compact_argv(char **argv, t_quote_token ***quote_chains_ptr)
{
	char			**new_argv;
	t_quote_token	**new_chains;
	int				count;
	int				i;

	count = 0;
	i = -1;
	while (argv[++i])
		if (argv[i][0] != '\0')
			count++;
	if (count == 0)
		return (NULL);
	new_argv = malloc(sizeof(char *) * (count + 1));
	new_chains = malloc(sizeof(t_quote_token *) * (count + 1));
	if (!new_argv || !new_chains)
		return (free(new_argv), free(new_chains), NULL);
	copy_non_empty(argv, new_argv, *quote_chains_ptr, new_chains);
	free(argv);
	if (*quote_chains_ptr)
		free(*quote_chains_ptr);
	*quote_chains_ptr = new_chains;
	return (new_argv);
}

char	*expand_segment(char *arg, int pos, t_quote_token *cur,
		t_expand_ctx *ctx)
{
	char	*segment;
	char	*exp;

	segment = ft_substr(arg, pos, cur->length);
	if (!segment)
		return (NULL);
	if (cur->qtype == SINGLE_QUOTE)
		return (segment);
	exp = expand_vars(segment, ctx->env, ctx->last_status);
	free(segment);
	return (exp);
}

char	*process_quote_chain(char *arg, t_quote_token *chain,
		t_expand_ctx *ctx)
{
	char			*new;
	char			*segment;
	char			*tmp;
	t_quote_token	*cur;
	int				pos;

	pos = 0;
	new = ft_strdup("");
	cur = chain;
	while (cur)
	{
		segment = expand_segment(arg, pos, cur, ctx);
		if (!segment)
			return (free(new), NULL);
		tmp = ft_strjoin(new, segment);
		free(new);
		free(segment);
		new = tmp;
		pos += cur->length;
		cur = cur->next;
	}
	return (new);
}
