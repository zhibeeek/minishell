/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adavitas <adavitas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 08:32:45 by zzhyrgal          #+#    #+#             */
/*   Updated: 2025/11/17 18:55:09 by adavitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "tokenizer.h"

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_APPEND,
	NODE_HEREDOC,
	NODE_INVALID,
}	t_node_type;

typedef struct s_heredoc
{
	char				*delimiter;
	char				*temp_filename;
	struct s_heredoc	*next;
}	t_heredoc;

typedef struct s_redir
{
	t_node_type		type;
	char			*filename;
	struct s_redir	*next;
}	t_redir;

typedef struct s_ast
{
	t_node_type		type;
	char			**argv;
	t_quote_token	**quote_chains;
	t_redir			*redirections;
	t_heredoc		*heredocs;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

char			**store_avs(t_token **tokens);
t_quote_token	**store_quote_types(t_token *tokens);
t_ast			*create_ast_node(t_node_type type);
t_ast			*create_command_node(t_token **tokens);
t_redir			*create_redir(t_node_type type, t_token **tokens);
t_ast			*create_pipe_node(t_node_type type, t_ast *left,
					t_ast *right);
t_quote_type	get_token_quote_type(t_token *token);
int				is_redirection(t_token_type type);
t_node_type		detect_redir_type(t_token *tokens);
void			append_redir(t_redir **redir_list, t_redir *new_redir);
void			append_heredoc(t_heredoc **heredoc_list,
					t_heredoc *new_heredoc);
int				size_words(t_token *tokens);
t_ast			*parse_command(t_token **tokens, t_heredoc **heredoc_list);
t_ast			*parse_pipeline(t_token **tokens, t_heredoc **heredoc_list);
t_ast			*syntax_analysis(t_token **token_list, t_heredoc *heredocs);
int				syntax_check(t_token **tokens);
void			free_ast(t_ast *ast);
void			free_heredoc_list(t_heredoc *heredocs);

#endif