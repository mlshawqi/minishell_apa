/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machaouk <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-21 09:56:57 by machaouk          #+#    #+#             */
/*   Updated: 2025-05-21 09:56:57 by machaouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	hanlde_shortcut(char *str, char *path)
{
	char	*new_path;

	new_path = ft_strjoin(str, ft_strchr(path, '~') + 1);
	if (chdir(new_path) == -1)
	{
		print_cmd_error("minishell: cd", "No such file or directory", new_path);
		free_str_null(&new_path);
		g_last_exit_code = 1;
		return (1);
	}
	free_str_null(&new_path);
	return (0);
}

static int	handle_home_shortcut(t_env *env, char *str)
{
	while (env)
	{
		if (ft_strcmp(env->name, "HOME") == 0)
		{
			if (str)
			{
				if (hanlde_shortcut(env->value, str) == 1)
					return (-1);
			}
			else if ((env->value[0] != '\0') && chdir(env->value) == -1)
			{
				print_cmd_error("minishell: cd", "No such file or directory",
					env->value);
				return (-1);
			}
			return (0);
		}
		env = env->next;
	}
	print_cmd_error("minishell: cd", "HOME not set", NULL);
	return (-1);
}

void	update_pwd(t_env **env, char *path, char hint)
{
	t_env	*lst;

	lst = *env;
	while (lst)
	{
		if ((hint == 'P' && (ft_strcmp(lst->name, "PWD") == 0)) || (hint == 'O'
				&& (ft_strcmp(lst->name, "OLDPWD") == 0)))
		{
			if (lst->value)
				free_str_null(&lst->value);
			if (!path)
				lst->value = ft_strdup("");
			else
				lst->value = ft_strdup(path);
			break ;
		}
		lst = lst->next;
	}
}

int	update_pwd2(t_data *data, t_env **env)
{
	char	*pwd;
	char	*tmp;

	pwd = NULL;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		if (data->pwd)
			pwd = ft_strjoin(data->pwd, "/..");
		print_cmd_error("cd",
			"error retrieving current directory: "
			"getcwd: cannot access parent directories: "
			"No such file or directory",
			NULL);
	}
	update_pwd(env, pwd, 'P');
	update_pwd(&data->export, pwd, 'P');
	if (data->pwd)
	{
		free_str_null(&data->pwd);
		data->pwd = ft_strdup(pwd);
	}
	if (pwd)
		free_str_null(&pwd);
	return (0);
}

int	run_cd(t_data *data, t_env **env, char **args)
{
	if ((!args || !args[0]) || (args[0][0] == '~') && !args[1])
	{
		if (args && args[0] && (ft_strcmp(args[0], "~") == 0))
		{
			if (handle_home_shortcut(*env, NULL) == -1)
				return (1);
		}
		else
		{
			if (handle_home_shortcut(*env, args[0]) == -1)
				return (1);
		}
	}
	else if (args[1])
	{
		print_cmd_error("minishell: cd", "too many arguments", NULL);
		return (1);
	}
	else if (chdir(*args) == -1)
	{
		print_cmd_error("minishell: cd", "No such file or directory", *args);
		return (1);
	}
	return (0);
}
