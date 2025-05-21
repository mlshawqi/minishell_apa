/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machaouk <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-21 09:57:11 by machaouk          #+#    #+#             */
/*   Updated: 2025-05-21 09:57:11 by machaouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cd_cmd(char **args, t_env **env, t_data *data)
{
	char	*old;

	old = getcwd(NULL, 0);
	if (!old && data->pwd)
		old = ft_strdup(data->pwd);
	if (run_cd(data, env, args) == 1)
	{
		if (old)
			free_str_null(&old);
		return (1);
	}
	update_pwd(env, old, 'O');
	update_pwd(&data->export, old, 'O');
	if (old)
		free_str_null(&old);
	update_pwd2(data, env);
	return (0);
}

static int	run_pwd(t_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		write(1, cwd, ft_strlen(cwd));
		write(1, "\n", 1);
		if (data->pwd)
			free_str_null(&data->pwd);
		data->pwd = cwd;
	}
	else
	{
		if (data->pwd != NULL)
		{
			write(1, data->pwd, ft_strlen(data->pwd));
			write(1, "\n", 1);
		}
		else
		{
			print_cmd_error("pwd", "getcwd failed", NULL);
			return (1);
		}
	}
	return (0);
}

int	pwd_cmd(t_data *data, char **args)
{
	if (args && args[0])
	{
		if (args && (ft_strlen(args[0]) > 1) && args[0][0] == '-')
		{
			print_cmd_error("minishell\npwd", "invalid option", args[0]);
			return (2);
		}
	}
	return (run_pwd(data));
}

static int	not_valid(char *str)
{
	int	i;

	i = 0;
	if (str && str[0] == '-')
	{
		write(2, "env: invalid option -- '", 25);
		write(2, str, ft_strlen(str));
		write(2, "'\n", 2);
		g_last_exit_code = 125;
		return (125);
	}
	if (str && (str[0] == '\0' || str[0] != '\0'))
	{
		write(2, "env: ‘", 8);
		write(2, str, ft_strlen(str));
		write(2, "’: No such file or directory\n", 31);
		g_last_exit_code = 127;
		return (127);
	}
	return (0);
}

int	env_cmd(t_env *lst, char **arg)
{
	if (arg && arg[0])
	{
		not_valid(arg[0]);
		return (g_last_exit_code);
	}
	while (lst != NULL)
	{
		if (lst->name && lst->value && (ft_strcmp(lst->name, "_") != 0))
		{
			write(1, lst->name, ft_strlen(lst->name));
			write(1, "=", 1);
			write(1, lst->value, ft_strlen(lst->value));
			write(1, "\n", 1);
		}
		lst = lst->next;
	}
	write(1, "_=/usr/bin/env\n", 15);
	return (0);
}
