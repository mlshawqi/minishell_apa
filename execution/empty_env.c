/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machaouk <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-21 09:57:27 by machaouk          #+#    #+#             */
/*   Updated: 2025-05-21 09:57:27 by machaouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	valide_shelvl(char *str)
{
	int	i;
	int	sign;

	i = 0;
	sign = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 1 || str[i] == ' ' || str[i] == '\t'
			|| str[i] == '+')
		{
			if (str[i] == '+')
				sign++;
			i++;
		}
		else
			return (1);
	}
	if (sign > 1)
		return (1);
	return (0);
}

int	ft_atoi_env(char *str)
{
	int	result;
	int	i;

	result = 0;
	i = 0;
	if (valide_shelvl(str) == 1)
		return (0);
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32 || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		if (result >= 1000)
			return (print_cmd_error("warning", "shell level (1001) too high,"
					"resetting to 1", NULL), 1);
		i++;
	}
	return (result + 1);
}

void	set_shell_lvl_export(t_env *env, t_env **export)
{
	t_env	*tmp;

	while (env)
	{
		if ((ft_strcmp(env->name, "SHLVL") == 0) && (env->value))
		{
			tmp = *export;
			while (tmp)
			{
				if (ft_strcmp(tmp->name, "SHLVL") == 0)
				{
					if (tmp->value)
					{
						free_str_null(&tmp->value);
						tmp->value = ft_strdup(env->value);
					}
					return ;
				}
				tmp = tmp->next;
			}
		}
		env = env->next;
	}
	link_node(export, creat_node("SHLVL", "1"));
}

void	set_shell_lvl(t_env *env)
{
	t_env	*tmp;
	int		nbr;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, "SHLVL") == 0)
		{
			if (tmp->value)
			{
				nbr = ft_atoi_env(tmp->value);
				if (tmp->value)
					free_str_null(&tmp->value);
				tmp->value = ft_itoa(nbr);
			}
			return ;
		}
		tmp = tmp->next;
	}
	link_node(&env, creat_node("SHLVL", "1"));
}

void	init_env_defaults(t_data *data)
{
	char	*pwd;
	char	*path;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		print_cmd_error("pwd", strerror(errno), NULL);
	link_node(&data->env, creat_node("PWD", pwd));
	link_node(&data->env, creat_node("SHLVL", "0"));
	path = ft_strdup("/home/machaouk/.local/bin"
			":/usr/local/sbin:/usr/local/bin"
			":/usr/sbin:/usr/bin:/sbin:/bin");
	link_node(&data->env, creat_node("PATH", path));
	link_node(&data->env, creat_node("OLDPWD", NULL));
	link_node(&data->export, creat_node("PWD", pwd));
	link_node(&data->export, creat_node("SHLVL", "0"));
	path = ft_strdup("/home/machaouk/.local/bin"
			":/usr/local/sbin:/usr/local/bin"
			":/usr/sbin:/usr/bin:/sbin:/bin");
	link_node(&data->export, creat_node("PATH", path));
	link_node(&data->export, creat_node("OLDPWD", NULL));
}
