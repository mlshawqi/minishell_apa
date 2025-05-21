#include "../minishell.h"

// bool	clean_up_old_file_ref(t_in_out_fds *io, bool is_input)
// {
// 	if (is_input == true && io->infile)
// 	{
// 		if (io->fd_in == -1 || (io->outfile && io->fd_out == -1))
// 			return (false);
// 		if (io->heredoc_delimiter != NULL)
// 		{
// 			free_str(io->heredoc_delimiter);
// 			io->heredoc_delimiter = NULL;
// 			unlink(io->infile);
// 		}
// 		free_str(io->infile);
// 		close(io->fd_in);
// 	}
// 	else if (is_input == false && io->outfile)
// 	{
// 		if (io->fd_out == -1 || (io->infile && io->fd_in == -1))
// 			return (false);
// 		free_str(io->outfile);
// 		close(io->fd_out);
// 	}
// 	return (true);
// }

int	open_file(t_in_out_fds *io, char *file, char *original_filename)
{
	int word_count;

	word_count = count_words(file);
	if (word_count != 1 || !file || file[0] == '\0')
	{
		if (g_last_exit_code == 0)
			print_command_error(original_filename, NULL, "ambiguous redirect", false);
		g_last_exit_code = 1;
		return (0);
	}
	io->filename = ft_strdup(file);
	return (0);
}

t_in_out_fds    *new_node_redirection(t_redirections type)
{
	t_in_out_fds   *new;

	new = malloc(sizeof(t_in_out_fds));
	if(!new)
	    return NULL;
	new->type = type;
	new->filename = NULL;
	new->fd = -1;
	new->heredoc_delimiter = NULL;
	new->heredoc_quotes = false;
	new->next = NULL;
	return (new);
}

void	link_node_redirection(t_in_out_fds **head, t_in_out_fds	*new_node)
{
	t_in_out_fds	*current;

	if (*head == NULL)
		*head = new_node;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}	
int	count_words(char *str)
{
	int i = 0;
	int words = 0;
	bool in_word = false;

	while (str[i])
	{
		if (!in_word && str[i] != ' ')
		{
			in_word = true;
			words++;
		}
		else if (str[i] == ' ')
			in_word = false;
		i++;
	}
	return words;
}

int	handle_input_redirection(t_cmd **last_cmd, t_separation **token_lst)
{
	t_separation	*temp;
	t_cmd			*cmd;
	t_in_out_fds		*red;
	int	i;

	temp = *token_lst;
	if (g_last_exit_code == 1)
	{
		if (temp->next && temp->next->next)
			*token_lst = temp->next->next;
		else if (temp->next)
			*token_lst = temp->next;
		return (0);
	}
	i = 0;
	cmd = get_last_command(*last_cmd);
	init_cmd_in_out(cmd);
	red = new_node_redirection(REDIR_IN);
	if(!red)
		return (malloc_error("t_in_out_fds"));
	i = open_file(red, temp->next->str, temp->next->str_copy);
	link_node_redirection(&cmd->io_fds, red);
	if (temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*token_lst = temp;
	return (i);
}
