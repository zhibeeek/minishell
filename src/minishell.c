/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zzhyrgal <zzhyrgal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 18:44:19 by adavitas          #+#    #+#             */
/*   Updated: 2025/11/20 18:18:16 by zzhyrgal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_line(char *line, t_env *env, int last_status,
		t_heredoc *heredocs)
{
	t_token	*tokens;
	t_token	*tokens_head;
	t_ast	*ast;
	int		status;

	tokens = tokenize_input(line);
	if (!tokens)
	{
		free_heredoc_list(heredocs);
		return (last_status);
	}
	tokens_head = tokens;
	ast = syntax_analysis(&tokens, heredocs);
	free_tokens(&tokens_head);
	if (!ast)
	{
		free_heredoc_list(heredocs);
		return (2);
	}
	expand_ast(ast, env, last_status);
	status = execute_ast(ast, env);
	free_ast(ast);
	return (status);
}

static int	handle_input(char *line)
{
	if (!line)
	{
		printf("exit\n");
		return (0);
	}
	return (1);
}

static int	process_input_line(char *line, t_env *env, int last_status)
{
	t_heredoc	*heredocs;

	add_history(line);
	g_signal_received = 0;
	heredocs = process_heredoc_input(line);
	if (g_signal_received == SIGINT)
		return (130);
	return (process_line(line, env, last_status, heredocs));
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*env;
	int		last_status;

	(void)argc;
	(void)argv;
	env = init_env(envp);
	setup_signals();
	last_status = 0;
	while (1)
	{
		line = readline("minishell$ ");
		if (!handle_input(line))
			break ;
		if (*line)
			last_status = process_input_line(line, env, last_status);
		free(line);
	}
	free_env(env);
	rl_clear_history();
	return (last_status);
}
