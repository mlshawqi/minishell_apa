#include "../minishell.h"

void	initialize_no_arg_commands(t_data *data)
{
	t_cmd	*cmd;

	if (!data || !data->cmd)
		return ;
	cmd = data->cmd;
	while (cmd && cmd->command)
	{
		if (!cmd->args)
		{
			cmd->args = malloc(sizeof(*cmd->args) * 2);
			cmd->args[0] = ft_strdup(cmd->command);
			cmd->args[1] = NULL;
		}
		cmd = cmd->next;
	}
	cmd = get_last_command(data->cmd);
}

int	ft_create_commands(t_data *data, t_separation *token)
{
	t_separation	*cur;
	int	value;

	cur = token;
	value = 0;
	if (cur->type == END)
		return (value);
	while (cur->next != NULL)
	{
		if (cur == token)
			add_command_to_list(&data->cmd, create_command(false));
		if (cur->type == WORD || cur->type == VAR)
			parse_word_token(&data->cmd, &cur);
		else if (cur->type == INPUT)
			value = handle_input_redirection(&data->cmd, &cur);
		else if (cur->type == TRUNC)
			value = parse_trunc(&data->cmd, &cur);
		else if (cur->type == HEREDOC)
			value = process_heredoc(data, &data->cmd, &cur);
		else if (cur->type == APPEND)
			value = handle_append_redirection(&data->cmd, &cur);
		else if (cur->type == PIPE)
			handle_pipe(&data->cmd, &cur);
		else if (cur->type == END)
			break ;
	}
	initialize_no_arg_commands(data);
	return (value);
}
