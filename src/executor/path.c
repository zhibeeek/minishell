/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adavitas <adavitas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 18:43:59 by adavitas          #+#    #+#             */
/*   Updated: 2025/11/11 18:44:00 by adavitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*search_in_paths(char **path_arr, char *command)
{
	char	*small_path;
	char	*path;
	int		index;

	index = -1;
	while (path_arr[++index])
	{
		small_path = ft_strjoin(path_arr[index], "/");
		path = ft_strjoin(small_path, command);
		free(small_path);
		if (access(path, F_OK) == 0)
		{
			free_array(path_arr);
			return (path);
		}
		free(path);
	}
	free_array(path_arr);
	return (NULL);
}

char	*get_command_path(char *command, t_env *env)
{
	char	**path_arr;
	char	*env_path;

	if (!command || !*command)
		return (NULL);
	if (ft_strchr(command, '/'))
		return (ft_strdup(command));
	env_path = get_env_value(env, "PATH");
	if (!env_path)
		return (NULL);
	path_arr = ft_split(env_path, ':');
	return (search_in_paths(path_arr, command));
}
