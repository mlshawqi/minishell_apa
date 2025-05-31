/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machaouk <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-21 09:58:08 by machaouk          #+#    #+#             */
/*   Updated: 2025-05-21 09:58:08 by machaouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	run_builtin_if_exists(t_data *data, t_cmd *cmd)
{
	if (ft_strcmp(cmd->command, "pwd") == 0)
		g_last_exit_code = pwd_cmd(data, cmd->args + 1);
	else if (ft_strcmp(cmd->command, "echo") == 0)
		g_last_exit_code = echo_cmd(cmd->args + 1);
	else if (ft_strcmp(cmd->command, "cd") == 0)
		g_last_exit_code = cd_cmd(cmd->args + 1, &data->env, data);
	else if (ft_strcmp(cmd->command, "env") == 0)
		g_last_exit_code = env_cmd(data->env, cmd->args + 1);
	else if (ft_strcmp(cmd->command, "exit") == 0)
		g_last_exit_code = exit_cmd(data, cmd->args + 1);
	else if (ft_strcmp(cmd->command, "export") == 0)
		g_last_exit_code = export_cmd(data, &data->env, &data->export, cmd->args
				+ 1);
	else if (ft_strcmp(cmd->command, "unset") == 0)
		g_last_exit_code = unset_cmd(&data->env, &data->export, cmd->args + 1);
	else
		return (1);
	return (0);
}

void	handle_fork_sig(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		g_last_exit_code = 130;
	}
	if (sig == SIGQUIT)
	{
		write(1, "Quit (core dumped)\n", 19);
		g_last_exit_code = 131;
	}
}

int	execute_with_pipes(t_data *data, int npipe)
{
	t_cmd	*tmp;
	int		**pipefd;
	int		i;

	tmp = data->cmd;
	pipefd = allocate_pipes(npipe);
	if (!pipefd)
		return (print_cmd_error("minishell: pipe", strerror(errno), NULL), 1);
	i = 0;
	set_child_sig();
	while (tmp)
	{
		if (init_or_count_pipes(tmp, 1) == -1)
			return (malloc_error("t_pipex"));
		tmp->pipex->fork_pid = fork();
		if (tmp->pipex->fork_pid == 0)
			handle_child_process(data, tmp, pipefd, i);
		tmp = tmp->next;
		i++;
	}
	close_pipes(pipefd, npipe);
	free_pipes(pipefd);
	return (wait_for_all(data));
}

int	one_command(t_data *data)
{
	if (init_or_count_pipes(data->cmd, 1) == -1)
		return (malloc_error("t_pipex pipex"));
	if (!data->cmd->io_fds)
	{
		if (data->cmd->command)
		{
			if (run_builtin_if_exists(data, data->cmd) == 1)
				g_last_exit_code = ft_execve(data, data->cmd);
		}
	}
	else
		handle_redirections(data, data->cmd);
	return (g_last_exit_code);
}

int	execution(t_data *data)
{
	int	npipe;

	npipe = init_or_count_pipes(data->cmd, 0);
	if (init_env_arr(data, 1) != 0)
		return (1);
	if (!npipe)
	{
		g_last_exit_code = one_command(data);
		init_env_arr(data, 0);
	}
	else
	{
		g_last_exit_code = execute_with_pipes(data, npipe);
		set_signals();
	}
	return (g_last_exit_code);
}
