/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adavitas <adavitas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 00:21:32 by zzhyrgal          #+#    #+#             */
/*   Updated: 2025/11/20 22:49:02 by adavitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int	process_unquoted(t_token *token, char *input, int pos)
{
	int				len;
	char			*content;
	char			*temp;
	t_quote_token	*qnode;

	len = 0;
	while (input[pos + len] && input[pos + len] != '\'' && input[pos
			+ len] != '"' && !deliminator(input[pos + len]))
		len++;
	content = ft_substr(input, pos, len);
	if (!content)
		return (0);
	temp = ft_strjoin(token->value, content);
	free(token->value);
	free(content);
	if (!temp)
		return (0);
	token->value = temp;
	qnode = create_quote_node(NO_QUOTE, len);
	append_quote_to_chain(&token->quote, qnode);
	return (len);
}

int	handle_pipe(t_token **token_list, char *input)
{
	int		len;
	t_token	*token;
	char	*pipe_str;

	len = 0;
	if (input[0] == '|')
		len = 1;
	pipe_str = ft_substr(input, 0, len);
	if (!pipe_str)
		return (0);
	token = create_token(pipe_str, TOKEN_PIPE);
	if (!token)
	{
		free(pipe_str);
		return (0);
	}
	add_token_back(token_list, token);
	free(pipe_str);
	return (len);
}

int	handle_redirect(t_token **token_list, char *input)
{
	int				len;
	t_token_type	type;
	char			*redir;
	t_token			*token;

	if ((input[0] == '>' && input[1] == '>') || (input[0] == '<'
			&& input[1] == '<'))
		len = 2;
	else
		len = 1;
	redir = ft_substr(input, 0, len);
	if (!redir)
		return (0);
	type = get_op_type(redir);
	token = create_token(redir, type);
	if (!token)
	{
		free(redir);
		return (0);
	}
	add_token_back(token_list, token);
	free(redir);
	return (len);
}

t_token	*create_empty_node(void)
{
	t_token	*a_token;

	a_token = malloc(sizeof(t_token));
	if (!a_token)
		return (NULL);
	a_token->value = NULL;
	a_token->type = TOKEN_WORD;
	a_token->quote = NULL;
	a_token->next = NULL;
	return (a_token);
}

int	handle_word(t_token **token_list, char *input)
{
	int		pos;
	int		result;
	t_token	*token;

	token = create_empty_node();
	if (!token)
		return (0);
	pos = 0;
	while (input[pos] && deliminator(input[pos]))
		pos++;
	while (input[pos] && !deliminator(input[pos]))
	{
		result = 0;
		if (input[pos] == '\'')
			result = process_single_quote(token, input, pos);
		else if (input[pos] == '"')
			result = process_double_quote(token, input, pos);
		else
			result = process_unquoted(token, input, pos);
		if (result < 0)
			return (0);
		pos += result;
	}
	add_token_back(token_list, token);
	return (pos);
}
