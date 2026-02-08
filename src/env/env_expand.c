/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adavitas <adavitas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 23:08:53 by adavitas          #+#    #+#             */
/*   Updated: 2025/11/17 18:55:09 by adavitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_var_value(char *var_name, t_env *env, int last_status)
{
	int		len;
	char	*key;
	char	*value;

	len = 0;
	if (var_name[0] == '?')
		return (ft_itoa(last_status));
	while (var_name[len] && (ft_isalnum(var_name[len]) || var_name[len] == '_'))
		len++;
	if (len == 0)
		return (ft_strdup(""));
	key = ft_substr(var_name, 0, len);
	if (!key)
		return (ft_strdup(""));
	value = get_env_value(env, key);
	free(key);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

static char	*expand_variable(char *str, int *i, t_env *env, int last_status)
{
	int		var_len;
	char	*var_value;

	(*i)++;
	if (str[*i] == '\0')
		return (ft_strdup("$"));
	var_len = 0;
	if (str[*i] == '?')
		var_len = 1;
	else
	{
		while (str[*i + var_len] && (ft_isalnum(str[*i + var_len])
				|| str[*i + var_len] == '_'))
			var_len++;
	}
	if (var_len == 0)
	{
		(*i)--;
		return (ft_strdup("$"));
	}
	var_value = get_var_value(&str[*i], env, last_status);
	*i += var_len - 1;
	return (var_value);
}

static char	*expand_tilde(char *str, int *i, t_env *env)
{
	char	*home;

	(*i)++;
	if (str[*i] == '\0' || str[*i] == '/')
	{
		home = get_env_value(env, "HOME");
		if (!home)
			return (ft_strdup("~"));
		(*i)--;
		return (ft_strdup(home));
	}
	(*i)--;
	return (ft_strdup("~"));
}

static char	*process_char(char *result, char *str, int *i, t_expand_ctx *ctx)
{
	char	*temp;
	char	*var_value;

	var_value = NULL;
	if (str[*i] == '~' && (*i == 0 || str[*i - 1] == ' ' || str[*i - 1] == ':'))
		var_value = expand_tilde(str, i, ctx->env);
	else if (str[*i] == '$' && str[*i + 1])
		var_value = expand_variable(str, i, ctx->env, ctx->last_status);
	temp = result;
	if (var_value)
		result = ft_strjoin(result, var_value);
	if (!var_value)
		result = ft_strjoin_char(result, str[*i]);
	free(temp);
	if (var_value)
		free(var_value);
	return (result);
}

char	*expand_vars(char *str, t_env *env, int last_status)
{
	char			*result;
	int				i;
	t_expand_ctx	ctx;

	if (!str)
		return (NULL);
	ctx.env = env;
	ctx.last_status = last_status;
	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		result = process_char(result, str, &i, &ctx);
		i++;
	}
	return (result);
}
