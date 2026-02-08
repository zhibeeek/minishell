/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adavitas <adavitas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 18:55:37 by zzhyrgal          #+#    #+#             */
/*   Updated: 2025/11/17 20:57:50 by adavitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "libft.h"

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_UNKNOWN,
}	t_token_type;

typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
}	t_quote_type;

typedef struct s_quote_token
{
	t_quote_type			qtype;
	int						length;
	struct s_quote_token	*next;
}	t_quote_token;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	t_quote_token		*quote;
	struct s_token		*next;
}	t_token;

t_token			*create_token(char *value, t_token_type type);
t_token			*create_empty_node(void);
t_quote_token	*create_quote_node(t_quote_type qtype, int len);
void			add_token_back(t_token **token_list, t_token *new_node);
void			free_quote_chain(t_quote_token *q);
void			free_tokens(t_token **head);

int				tokenize(t_token **token_list, char *str);
t_token			*tokenize_input(char *input);

int				handle_pipe(t_token **token_list, char *input);
int				handle_redirect(t_token **token_list, char *input);
int				handle_word(t_token **token_list, char *input);

int				process_single_quote(t_token *token, char *input, int pos);
int				process_double_quote(t_token *token, char *input, int pos);
int				process_unquoted(t_token *token, char *input, int pos);
int				find_closing_quote(char *input, int start, char quote_char);
int				handle_quote_content(t_token *token, char *content);
void			append_quote_to_chain(t_quote_token **chain,
					t_quote_token *new_node);

int				is_redirect(const char *str);
t_token_type	get_op_type(char *token);
int				deliminator(char c);
void			print_tokens(t_token *list);

#endif