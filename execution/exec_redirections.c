/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machaouk <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-21 09:57:41 by machaouk          #+#    #+#             */
/*   Updated: 2025-05-21 09:57:41 by machaouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_files(t_in_out_fds *redirec)
{
	if (redirec->fd == -1)
	{
		if (!redirec->filename || (redirec->filename
				&& redirec->filename[0] == '\0' || redirec->filename[0] == '$') )
		{
			print_cmd_error("minishell", "ambiguous redirect", redirec->filename);
			return (1);
		}
		else if (redirec->type == REDIR_IN)
			redirec->fd = open(redirec->filename, O_RDONLY);
		else if (redirec->type == REDIR_OUT)
		{
			redirec->fd = open(redirec->filename, O_WRONLY | O_CREAT | O_TRUNC,
					0664);
		}
		else if (redirec->type == REDIR_APPEND)
		{
			redirec->fd = open(redirec->filename, O_WRONLY | O_CREAT | O_APPEND,
					0664);
		}
		if (redirec->fd == -1)
		{
			return (print_cmd_error("minishell", strerror(errno), redirec->filename), 1);
		}
	}
	return (0);
}

static int	loop_in_redirec(t_data *data, t_cmd *cmd)
{
	t_in_out_fds	*tmp;

	tmp = cmd->io_fds;
	while (tmp)
	{
		if (open_files(tmp) == 1)
			return (1);
		if (dup_redirect(tmp) == 1)
			return (1);
		tmp = tmp->next;
	}
	if (cmd->command)
	{
		if (run_builtin_if_exists(data, cmd) == 1)
			g_last_exit_code = ft_execve(data, cmd);
	}
	return (0);
}

int	handle_redirections(t_data *data, t_cmd *cmd)
{
	int	saved_stdout;
	int	saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (loop_in_redirec(data, cmd) == 1)
	{
		g_last_exit_code = 1;
		dup2(saved_stdout, STDOUT_FILENO);
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdout);
		close(saved_stdin);
		return (1);
	}
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
	return (0);
}
