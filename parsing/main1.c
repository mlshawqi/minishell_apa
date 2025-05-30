#include "../minishell.h"

int g_last_exit_code = 0;

void	minishell_interactive(t_data *data)
{
	while (1)
	{
		set_signals();
		// configure_interactive_signals();
		data->user_input = readline("minishell~$ ");
		if (!data->user_input)
		{
			printf("exit\n");
			break ;
		}
		if(process_user_input(data) == true)
		{
			if(data->cmd)
				g_last_exit_code = execution(data);
		}
		cleanup_shell_data(data, false);
	}
}

void	minishell_noninteractive(t_data *data, char *arg)
{
	char	**user_inputs;
	int		i;

	user_inputs = ft_split(arg, ';');
	if (!user_inputs)
		exit_shell(data, EXIT_FAILURE);
	i = 0;
	if (user_inputs[i])
	{
		data->user_input = ft_strdup(user_inputs[i]);
		if(process_user_input(data) == true)
		{
			if(data->cmd)
				g_last_exit_code = execution(data);
		}
		cleanup_shell_data(data, false);
	}
	free_string_array(user_inputs);
}

int	main(int ac, char **av, char **env)
{
	t_data	data;

	ft_initialise_data(&data, env);
	if (!validate_startup_args(&data, ac, av)) {
	    exit_shell(&data, EXIT_FAILURE);
	}
	if (data.interactive)
	    minishell_interactive(&data);
	else
	    minishell_noninteractive(&data, av[2]);
	cleanup_shell_data(&data, true);
	exit (g_last_exit_code);
}
