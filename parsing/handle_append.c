#include "../minishell.h"

void	open_output_file_append(t_in_out_fds *io,
		char *file, char *original_filename)
{
	io->filename = ft_strdup(file);
	if (io->filename && io->filename[0] == '\0' && original_filename)
	{
		print_command_error(original_filename, NULL,
			"ambiguous redirect", false);
		return ;
	}
	// io->fd = open(io->filename, O_WRONLY | O_CREAT | O_APPEND, 0664);
	// if (io->fd == -1)
	// 	print_command_error(io->filename, NULL, strerror(errno), false);
}

int	handle_append_redirection(t_cmd **last_cmd, t_separation **token_lst)
{
	t_separation	*temp;
	t_cmd			*cmd;
	t_in_out_fds		*red;
	int	i;

	temp = *token_lst;
	// if (g_last_exit_code == 1)
	// {
	// 	if (temp->next && temp->next->next)
	// 		*token_lst = temp->next->next;
	// 	else if (temp->next)
	// 		*token_lst = temp->next;
	// 	return (0);
	// }
	i = 0;
	cmd = get_last_command(*last_cmd);
	init_cmd_in_out(cmd);
	red = new_node_redirection(REDIR_APPEND);
	if(!red)
		return (malloc_error("t_in_ut_fds"));
	i = open_file(red, temp->next->str, temp->next->str_copy);
	link_node_redirection(&cmd->io_fds, red);
	if (temp->next && temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*token_lst = temp;
	return (i);
}
