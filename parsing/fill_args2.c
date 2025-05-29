#include "../minishell.h"

char	*combine_vars(t_separation **token_node)
{
	t_separation	*current_token = *token_node;
	char			*result = ft_strdup(current_token->str);
	char			*tmp;

	while (current_token->next && current_token->next->type == VAR && current_token->next->join == true)
	{
		tmp = result;
		result = ft_strjoin(result, current_token->next->str);
		free(tmp);
		current_token = current_token->next;
	}

	*token_node = current_token;
	return (result);
}



int count_arguments(t_separation *current_token)
{
    int count = 0;
    char **split;
    int i;

    while (current_token && (current_token->type == WORD || current_token->type == VAR))
    {
        if (current_token->join == true)
        {
            while (current_token->next && current_token->next->type == VAR && current_token->next->join == true)
                current_token = current_token->next;
            count++;
        }
        else if (current_token->quoted)
        {
            count++;
        }
        else if (current_token->type == VAR && has_space(current_token->str) && !current_token->quoted)
        {
            split = ft_split(current_token->str, ' ');
            if (!split)
            {
                count++;
            }
            else
            {
                i = 0;
                while (split[i])
                {
                    count++;
                    i++;
                }
                free_string_array(split);
            }
        }
        else
        {
            count++;
        }

        current_token = current_token->next;
    }

    return count;
}



char	**copy_args_to_new_tab(int existing_len, char **new_args,
	t_cmd *last_cmd, t_separation *current_token)
{
	int		i = 0;
	int		j;
	char	**split;
	while (i < existing_len)
	{
		new_args[i] = ft_strdup(last_cmd->args[i]);
		if (!new_args[i])
			return (NULL);
		i++;
	}
	while (current_token && (current_token->type == WORD || current_token->type == VAR))
	{
		if (current_token->join == true)
		{
			new_args[i++] = combine_vars(&current_token);
			current_token = current_token->next;
		}
		else if (current_token->quoted)
		{
			new_args[i++] = ft_strdup(current_token->str);
			current_token = current_token->next;
		}
		else if (current_token->type == VAR && has_space(current_token->str))
		{
			split = ft_split(current_token->str, ' ');
			if (!split)
				return (NULL);
			j = 0;
			while (split[j])
				new_args[i++] = ft_strdup(split[j++]);
			free_string_array(split);
			current_token = current_token->next;
		}
		else
		{
			new_args[i++] = ft_strdup(current_token->str);
			current_token = current_token->next;
		}
	}
	new_args[i] = NULL;
	return (new_args);
}




void	remove_empty_variables(t_separation **tokens)
{
	t_separation	*current = *tokens;
	t_separation	*next;

	while (current)
	{
		next = current->next;
		if (current->type == VAR && current->str[0] == '\0' && current->is_var == false)
		{
			if (current == *tokens)
				*tokens = next;
			delete_token(current, free_str);
		}
		current = next;
	}
}

int initialize_echo_arguments(t_separation **token_node, t_cmd *last_cmd)
{
    int             nb_args;
    t_separation    *current_token;
    int             i = 0;
    char            **split;
    int             j;

    remove_empty_variables(token_node);
    current_token = *token_node;

    nb_args = count_arguments(current_token);
    last_cmd->args = malloc(sizeof(char *) * (nb_args + 2));
    if (!last_cmd->args)
        return (FAILURE);

    last_cmd->args[i++] = ft_strdup(last_cmd->command);

    while (current_token && (current_token->type == WORD || current_token->type == VAR))
    {
        if (current_token->join == true)
        {
            last_cmd->args[i++] = combine_vars(&current_token);
        }
        else if (current_token->quoted)
        {
            last_cmd->args[i++] = ft_strdup(current_token->str);
            current_token = current_token->next;
        }
        else if (current_token->type == VAR && has_space(current_token->str))
        {
            split = ft_split(current_token->str, ' ');
            if (!split)
			return (FAILURE);
			
            j = 0;
            while (split[j])
			last_cmd->args[i++] = ft_strdup(split[j++]);
			
            free_string_array(split);
            current_token = current_token->next;
        }
        else
        {
			last_cmd->args[i++] = ft_strdup(current_token->str);
            current_token = current_token->next;
        }
    }

    last_cmd->args[i] = NULL;
    *token_node = current_token;
    return (SUCCESS);
}


int	append_echo_arguments(t_separation **token_node, t_cmd *last_cmd)
{

	int				nb_args;
	int				len;
	char			**new_tab;
	t_separation	*current_token;
	int k = 0;

	remove_empty_variables(token_node);
	current_token = *token_node;

	nb_args = count_arguments(current_token);
	if (!last_cmd->args)
		len = 0;
	else
	{
		len = 0;
		while (last_cmd->args[len])
			len++;
	}
	new_tab = malloc(sizeof(char *) * (nb_args + len + 1));
	if (!new_tab)
		return (FAILURE);
	new_tab = copy_args_to_new_tab(len, new_tab, last_cmd, current_token);
	if (!new_tab)
		return (FAILURE);
	if (last_cmd->args)
	{
		while (k < len)
		{
			free(last_cmd->args[k]);
			k++;
		}
		free(last_cmd->args);
	}

	last_cmd->args = new_tab;
	while (current_token && (current_token->type == WORD || current_token->type == VAR))
		current_token = current_token->next;
	*token_node = current_token;

	return (SUCCESS);
}

