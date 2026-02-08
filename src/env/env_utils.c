/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adavitas <adavitas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 18:14:22 by adavitas          #+#    #+#             */
/*   Updated: 2025/11/11 18:14:23 by adavitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *env, char *key)
{
	t_env_var	*var;

	var = env->vars;
	while (var)
	{
		if (ft_strcmp(var->key, key) == 0)
			return (var->value);
		var = var->next;
	}
	return (NULL);
}

void	set_env_value(t_env *env, char *key, char *value)
{
	t_env_var	*var;
	t_env_var	*new_var;

	var = env->vars;
	while (var)
	{
		if (ft_strcmp(var->key, key) == 0)
		{
			free(var->value);
			var->value = ft_strdup(value);
			return ;
		}
		var = var->next;
	}
	new_var = malloc(sizeof(t_env_var));
	if (!new_var)
		return ;
	new_var->key = ft_strdup(key);
	new_var->value = ft_strdup(value);
	new_var->next = env->vars;
	env->vars = new_var;
	free_array(env->envp_array);
	env->envp_array = env_to_array(env);
}

void	unset_env_value(t_env *env, char *key)
{
	t_env_var	*var;
	t_env_var	*prev;

	var = env->vars;
	prev = NULL;
	while (var)
	{
		if (ft_strcmp(var->key, key) == 0)
		{
			if (prev)
				prev->next = var->next;
			else
				env->vars = var->next;
			free(var->key);
			free(var->value);
			free(var);
			free_array(env->envp_array);
			env->envp_array = env_to_array(env);
			return ;
		}
		prev = var;
		var = var->next;
	}
}

char	**env_to_array(t_env *env)
{
	char		**array;
	t_env_var	*var;
	int			count;
	int			i;
	char		*temp;

	count = 0;
	var = env->vars;
	while (var && ++count)
		var = var->next;
	array = malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	var = env->vars;
	i = 0;
	while (var)
	{
		temp = ft_strjoin(var->key, "=");
		array[i] = ft_strjoin(temp, var->value);
		free(temp);
		var = var->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}

void	free_env(t_env *env)
{
	t_env_var	*var;
	t_env_var	*next;

	var = env->vars;
	while (var)
	{
		next = var->next;
		free(var->key);
		free(var->value);
		free(var);
		var = next;
	}
	free_array(env->envp_array);
	free(env);
}
