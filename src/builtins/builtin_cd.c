/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adavitas <adavitas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 18:13:36 by adavitas          #+#    #+#             */
/*   Updated: 2025/11/11 18:13:39 by adavitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_pwd_vars(t_env *env, char *old_pwd)
{
	char	cwd[4096];

	set_env_value(env, "OLDPWD", old_pwd);
	if (getcwd(cwd, sizeof(cwd)))
		set_env_value(env, "PWD", cwd);
}

static char	*get_target_dir(char **args, t_env *env)
{
	char	*target;

	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		target = get_env_value(env, "HOME");
		if (!target)
		{
			print_error("cd", NULL, "HOME not set");
			return (NULL);
		}
		return (target);
	}
	if (ft_strcmp(args[1], "-") == 0)
	{
		target = get_env_value(env, "OLDPWD");
		if (!target)
		{
			print_error("cd", NULL, "OLDPWD not set");
			return (NULL);
		}
		printf("%s\n", target);
		return (target);
	}
	return (args[1]);
}

int	builtin_cd(char **args, t_env *env)
{
	char	*target;
	char	old_pwd[4096];

	if (args[1] && args[2])
	{
		print_error("cd", NULL, "too many arguments");
		return (1);
	}
	if (!getcwd(old_pwd, sizeof(old_pwd)))
		old_pwd[0] = '\0';
	target = get_target_dir(args, env);
	if (!target)
		return (1);
	if (chdir(target) == -1)
	{
		print_error("cd", target, strerror(errno));
		return (1);
	}
	update_pwd_vars(env, old_pwd);
	return (0);
}
