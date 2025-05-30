#include "../minishell.h"

void	free_command(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->command)
		free_str_null(&cmd->command);
	if (cmd->args)
		free_string_array(cmd->args);
	if (cmd->pipex)
	{
		if(cmd->pipex->path)
			free_str_null(&cmd->pipex->path);
		free(cmd->pipex);
		cmd->pipex = NULL;
	}
	if (cmd->io_fds)
		free_in_out(&cmd->io_fds);
}


void	free_command_list(t_cmd **cmd_list)
{
	t_cmd	*temp;

	temp = NULL;
	while (*cmd_list)
	{
		temp = (*cmd_list)->next;
		free_command(*cmd_list);
		free(*cmd_list);
		*cmd_list = NULL;
		*cmd_list = temp;
	}
	*cmd_list = NULL;
}
