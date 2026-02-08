/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_types.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zzhyrgal <zzhyrgal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 18:55:25 by zzhyrgal          #+#    #+#             */
/*   Updated: 2025/11/13 15:26:16 by zzhyrgal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int	is_redirect(const char *str)
{
	if (!str || !*str)
		return (0);
	if (str[0] == '>' || str[0] == '<')
		return (1);
	return (0);
}

t_token_type	get_op_type(char *token)
{
	if (!token)
		return (TOKEN_WORD);
	if (ft_strcmp(token, "|") == 0)
		return (TOKEN_PIPE);
	else if (ft_strcmp(token, "<") == 0)
		return (TOKEN_REDIR_IN);
	else if (ft_strcmp(token, ">") == 0)
		return (TOKEN_REDIR_OUT);
	else if (ft_strcmp(token, ">>") == 0)
		return (TOKEN_APPEND);
	else if (ft_strcmp(token, "<<") == 0)
		return (TOKEN_HEREDOC);
	return (TOKEN_WORD);
}
