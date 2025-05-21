/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machaouk <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-21 09:58:53 by machaouk          #+#    #+#             */
/*   Updated: 2025-05-21 09:58:53 by machaouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	appand_value(t_env *tmp, char *arg)
{
	char	*str;

	while (tmp)
	{
		if (ft_strncmp(tmp->name, arg, is_len(tmp->name, arg, '+')) == 0)
		{
			if (tmp->value)
			{
				str = tmp->value;
				tmp->value = ft_strjoin(str, (ft_strchr(arg, '=') + 1));
				free_str_null(&str);
			}
			else
				tmp->value = ft_strdup(ft_strchr(arg, '=') + 1);
			return (0);
		}
		tmp = tmp->next;
	}
	return (-1);
}

void	export_var(t_env **lst, char *var)
{
	int		len;
	int		found;
	t_env	*tmp;

	found = 0;
	tmp = *lst;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, var, is_len(tmp->name, var, '=')) == 0)
		{
			len = ft_strlen(ft_strchr(var, '=') + 1);
			if (tmp->value)
				free_str_null(&tmp->value);
			tmp->value = malloc(sizeof(char) * len + 1);
			if (tmp->value)
				ft_strlcpy(tmp->value, ft_strchr(var, '=') + 1, len + 1);
			else
				malloc_error("t_env");
			found++;
			break ;
		}
		tmp = tmp->next;
	}
	if (found == 0)
		ft_lstadd_back(lst, ft_lstnew(var));
}

void	swap_nodes(t_env *lst, t_env **env)
{
	t_env	*tmp;

	tmp = lst->next;
	lst->next = tmp->next;
	if (tmp->next)
		tmp->next->previous = lst;
	tmp->previous = lst->previous;
	if (lst->previous)
		lst->previous->next = tmp;
	else
		*env = tmp;
	tmp->next = lst;
	lst->previous = tmp;
}

static void	print_export(t_env *lst)
{
	while (lst != NULL)
	{
		if (lst->name && (ft_strcmp(lst->name, "_") != 0))
		{
			write(1, "declare -x ", 11);
			write(1, lst->name, ft_strlen(lst->name));
			if (lst->value)
			{
				write(1, "=\"", 2);
				write(1, lst->value, ft_strlen(lst->value));
				write(1, "\"", 1);
			}
			write(1, "\n", 1);
		}
		lst = lst->next;
	}
}

void	sort_env(t_env **env)
{
	t_env	*lst;
	int		len;

	len = ft_lstsize(*env);
	while (len)
	{
		lst = *env;
		while (lst->next != NULL)
		{
			if (lst->next && (ft_strcmp(lst->name, lst->next->name) > 0))
			{
				swap_nodes(lst, env);
			}
			else
				lst = lst->next;
		}
		len--;
	}
	print_export(*env);
	g_last_exit_code = 0;
}
