/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adavitas <adavitas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 18:14:48 by adavitas          #+#    #+#             */
/*   Updated: 2025/11/18 22:22:23 by adavitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_absolute_path(char **args, t_env *env)
{
	if (access(args[0], F_OK) == -1)
	{
		print_error(args[0], NULL, "No such file or directory");
		exit(127);
	}
	if (access(args[0], X_OK) == -1)
	{
		print_error(args[0], NULL, "Permission denied");
		exit(126);
	}
	execve(args[0], args, env->envp_array);
	print_error(args[0], NULL, "Is a directory");
	exit(126);
}

static void	execute_from_path(char **args, t_env *env)
{
	char	*path;

	path = get_command_path(args[0], env);
	if (!path)
	{
		print_error(args[0], NULL, "command not found");
		exit(127);
	}
	execve(path, args, env->envp_array);
	free(path);
	print_error(args[0], NULL, "Is a directory");
	exit(126);
}

void	execute_command(char **args, t_env *env)
{
	if (!args || !args[0])
	{
		print_error("minishell", NULL, "empty command");
		exit(127);
	}
	if (ft_strchr(args[0], '/'))
		execute_absolute_path(args, env);
	else
		execute_from_path(args, env);
}
