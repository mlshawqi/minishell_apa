/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machaouk <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-21 09:58:27 by machaouk          #+#    #+#             */
/*   Updated: 2025-05-21 09:58:27 by machaouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_str_null(char **str)
{
	if (!str || !(*str))
		return ;
	free(*str);
	*str = NULL;
}

void	set_child_sig(void)
{
	signal(SIGINT, handle_fork_sig);
	signal(SIGQUIT, handle_fork_sig);
}
void	check_status(int *status)
{
	if (WIFEXITED(*status))
		g_last_exit_code = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
	{
		if (WTERMSIG(*status) == SIGQUIT)
		{
			printf("Quit (core dumped)\n");
			g_last_exit_code = 131;
		}
		else if (WTERMSIG(*status) == SIGINT)
		{
			printf("\n");
			g_last_exit_code = 130;
		}
	}
}

int	ft_execve(t_data *data, t_cmd *cmd)
{
	int	pid;
	int	status;

	cmd->pipex->path = find_program_path(data->env, cmd->command);
	if (!cmd->pipex->path)
		return (g_last_exit_code);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if(pid < 0)
		return (print_cmd_error("minishell", "fork fail", NULL), 1);
	if (pid == 0)
	{
		set_child_sig();
		if (execve(cmd->pipex->path, data->cmd->args, data->env_arr) == -1)
			print_cmd_error("minishell: pipe", strerror(errno), NULL);
		g_last_exit_code= 127;
		cleanup_shell_data(data, true);
	}
	waitpid(pid, &status, 0);
	check_status(&status);
	// else
	// { 
	// 	waitpid(pid, &status, 0);
	// 	if (WIFEXITED(status))
	// 		g_last_exit_code = WEXITSTATUS(status);
	// 	return (g_last_exit_code);
	// }
	return (g_last_exit_code);
}
