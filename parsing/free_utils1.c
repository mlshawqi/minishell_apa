#include "../minishell.h"

void	free_in_out(t_in_out_fds **io)
{
	t_in_out_fds	*t;

	while(*io)
	{
		if ((*io)->heredoc_delimiter)
		{
			// unlink((*io)->filename);
			free_str_null(&(*io)->heredoc_delimiter);
		}
		if ((*io)->filename)
			free_str_null(&(*io)->filename);
		if ((*io)->fd != -1)
		{
			close((*io)->fd);
			(*io)->fd = -1;
		}
		t = (*io)->next;
		free((*io));
		(*io) = t;
	}
	free(*io);
}

void	free_string_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	if (array)
	{
		while (array[i])
		{
			if (array[i])
			{
				free_str_null(&array[i]);
				array[i] = NULL;
			}
			i++;
		}
		free(array);
		array = NULL;
	}
}

void	close_files_descriptors(t_cmd *cmds, bool should_close)
{
	t_in_out_fds	*tmp;

	if (cmds->io_fds)
	{
		tmp = cmds->io_fds;
		while(tmp)
		{
			if (tmp->fd != -1)
			{
				close(tmp->fd);
				tmp->fd = -1;
			}
			tmp = tmp->next;
		}
	}
}

void	free_env_list(t_env **list)
{
	t_env *p;

	
	if (list == NULL || *list == NULL)
		return ;
	while (*list != NULL)
	{
		p = (*list)->next;
		if((*list)->name)
		{
			free((*list)->name);
			(*list)->name = NULL;
		}
		if((*list)->value)
		{
			free((*list)->value);
			(*list)->value = NULL;
		}
		free(*list);
		*list = p;
	}
	free(*list);
	*list = NULL;
}

void free_env_array(char **env_arr)
{
    int i;

    if (!env_arr)
        return;

    i = 0;
    while (env_arr[i])
    {
        free_str_null(&env_arr[i]);
        env_arr[i] = NULL;
        i++;
    }
    free(env_arr);
}
void	execution_cleanup(t_data *data)
{

}
// void	stiil_allocat(t_data *data)
// {
// 	if(data->cmd)
// 	{
// 		printf("data.cmd\n");
// 		// if(data->cmd)
// 		// {
// 		// 	printf("data.cmd\n");
// 		// }
// 	}
// 	if(data->pwd)
// 	{
// 		printf("data.pwd\n");
// 	}
// 	if(data->export)
// 	{
// 		printf("data.export\n");
// 	}
// 	if(data->env_arr)
// 	{
// 		printf("data.env_arr\n");
// 	}
// 	if(data->env)
// 	{
// 		printf("data.env\n");
// 	}
// }

void	cleanup_shell_data(t_data *data, bool clear_history)
{
	if (!data)
		return ;
	free_str_null(&data->user_input);
	data->user_input = NULL;
	if(data->env_arr)
	{
		free_string_array(data->env_arr);
		data->env_arr = NULL;
	}
	clear_token_list(&data->token, &free_str);
	free_command_list(&data->cmd);
	// stiil_allocat(data);
	if (clear_history)
	{
		if(data->pwd)
			free_str_null(&data->pwd);
		if(data->env)
			free_env_list(&data->env);
		if(data->export)
			free_env_list(&data->export);
		rl_clear_history();
		exit (g_last_exit_code);
	}
}
