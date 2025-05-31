/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machaouk <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-21 09:59:35 by machaouk          #+#    #+#             */
/*   Updated: 2025-05-21 09:59:35 by machaouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	wait_for_all(t_data *data)
{
	t_cmd	*tmp;

	tmp = data->cmd;
	while (tmp)
	{
		waitpid(tmp->pipex->fork_pid, &tmp->pipex->status, 0);
		if (!tmp->next)
		{
			if (WIFEXITED(tmp->pipex->status))
				g_last_exit_code = WEXITSTATUS(tmp->pipex->status);
			return (g_last_exit_code);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	handle_pipe_redirections(t_data *data, t_cmd *cmd)
{
	t_in_out_fds	*tmp;

	tmp = cmd->io_fds;
	while (tmp)
	{
		if (tmp->fd == -1)
		{
			if (open_files(tmp) == 1)
			{
				g_last_exit_code = 1;
				return (1);
			}
		}
		if (dup_redirect(tmp) == 1)
			return (1);
		tmp = tmp->next;
	}
	if (cmd->command)
	{
		if (ft_strcmp(cmd->command, "exit") == 0)
			g_last_exit_code = exit_cmd_pipe(data, cmd->args + 1);
		if (run_builtin_if_exists(data, cmd) == 1)
			ft_execve_pipe(data, cmd);
	}
	return (g_last_exit_code);
}

int	ft_execve_pipe(t_data *data, t_cmd *cmd)
{
	cmd->pipex->path = find_program_path(data->env, cmd->command);
	if (!cmd->pipex->path)
		return (g_last_exit_code);
	if (execve(cmd->pipex->path, cmd->args, data->env_arr) == -1)
		perror("execve");
	g_last_exit_code = 1;
	return (EXIT_FAILURE);
}

int	execute_command(t_data *data, t_cmd *cmd)
{
	if (!cmd->io_fds)
	{
		if (cmd->command)
		{
			if (ft_strcmp(cmd->command, "exit") == 0)
				g_last_exit_code = exit_cmd_pipe(data, cmd->args + 1);
			else if (run_builtin_if_exists(data, cmd) == 1)
			{
				return (ft_execve_pipe(data, cmd));
			}
		}
	}
	else if (cmd->io_fds)
		handle_pipe_redirections(data, cmd);
	return (g_last_exit_code);
}

int	handle_child_process(t_data *data, t_cmd *cmd, int **pipes, int i)
{
	int	count;

	g_last_exit_code = 0;
	count = init_or_count_pipes(data->cmd, 0);
	if (!cmd->prev)
	{
		dup2(pipes[i][1], STDOUT_FILENO);
	}
	else if (!cmd->next)
	{
		dup2(pipes[i - 1][0], STDIN_FILENO);
	}
	else
	{
		dup2(pipes[i - 1][0], STDIN_FILENO);
		dup2(pipes[i][1], STDOUT_FILENO);
	}
	close_pipes(pipes, count);
	execute_command(data, cmd);
	free_pipes(pipes);
	cleanup_shell_data(data, true);
	return (0);
}
