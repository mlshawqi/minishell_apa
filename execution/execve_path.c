/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machaouk <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-21 09:58:22 by machaouk          #+#    #+#             */
/*   Updated: 2025-05-21 09:58:22 by machaouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	is_directory(char *cmd)
{
	struct stat	statbuf;

	if (stat(cmd, &statbuf) == -1)
		return (false);
	return (S_ISDIR(statbuf.st_mode));
}

char	*valid_path(char *cmd, char **arr)
{
	char	*tmp;
	char	*path;
	int		i;

	tmp = NULL;
	path = NULL;
	i = 0;
	while (arr && arr[i])
	{
		tmp = ft_strjoin(arr[i], "/");
		path = ft_strjoin(tmp, cmd);
		if (tmp)
			free_str_null(&tmp);
		if ((access(path, X_OK) == 0) && !is_directory(path))
			return (path);
		if (path)
			free_str_null(&path);
		i++;
	}
	print_cmd_error(cmd, "command not found", NULL);
	g_last_exit_code = 127;
	return (NULL);
}

char	*relative_path(t_env *env, char *cmd)
{
	char	**arr;
	char	*path;

	arr = NULL;
	path = NULL;
	while (env)
	{
		if (ft_strcmp(env->name, "PATH") == 0)
		{
			if (env->value && env->value[0] != '\0')
				arr = ft_split(env->value, ':');
		}
		env = env->next;
	}
	if (!arr)
	{
		print_cmd_error("minishell", "No such file or directory", cmd);
		g_last_exit_code = 127;
		return (NULL);
	}
	path = valid_path(cmd, arr);
	if (arr)
		free_string_array(arr);
	return (path);
}

static char	*path_in_directory(char *cmd)
{
	if (access(cmd, F_OK) != 0)
	{
		print_cmd_error("minishell", "No such file or directory", cmd);
		g_last_exit_code = 127;
		return (NULL);
	}
	if (is_directory(cmd))
	{
		print_cmd_error("minishell", "Is a directory", cmd);
		g_last_exit_code = 126;
		return (NULL);
	}
	if (access(cmd, X_OK) != 0)
	{
		print_cmd_error("minishell", "Permission denied", cmd);
		g_last_exit_code = 126;
		return (NULL);
	}
	return (ft_strdup(cmd));
}

char	*find_program_path(t_env *env, char *cmd)
{
	int		i;
	t_env	*tmp_env;

	i = 0;
	tmp_env = env;
	if (cmd == NULL || cmd[0] == '\0')
	{
		g_last_exit_code = 127;
		return (print_cmd_error("minishell", "command not found", ""), NULL);
	}
	if (ft_strcmp(cmd, ".") == 0)
	{
		print_cmd_error("minishell", "filename argument required", cmd);
		g_last_exit_code = 2;
		return (NULL);
	}
	while (cmd[i])
	{
		if (cmd[i] != '\0' && cmd[i] == '/')
			return (path_in_directory(cmd));
		i++;
	}
	return (relative_path(tmp_env, cmd));
}
