#include "../minishell.h"

static int	does_var_exist(t_env *env, char *var)
{
	while (env)
	{
		if (ft_strcmp(env->name, var) == 0 && env->value)
			return (0);
		env = env->next;
	}
	return (1);
}

static char	*find_env_var(t_env *env, char *var)
{
	while (env)
	{
		if (ft_strcmp(env->name, var) == 0 && env->value)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (NULL);
}

char	*clean_spaces(const char *str)
{
	int		len;
	char	*result;
	int		i = 0, j;
	int		in_word;

	if (!str)
		return (NULL);
	len = strlen(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0, j = 0;
	in_word = 0;
	while (isspace(str[i]))
		i++;
	while (str[i])
	{
		if (!isspace(str[i]))
		{
			result[j++] = str[i];
			in_word = 1;
		}
		else if (in_word)
		{
			result[j++] = ' ';
			in_word = 0;
			while (isspace(str[i + 1]))
				i++;
		}
		i++;
	}
	if (j > 0 && result[j - 1] == ' ')
		j--;
	result[j] = '\0';
	return (result);
}

char	*get_value(t_separation *token, char *str, t_env *env_list)
{
	char	*value;
	char	*var;

	var = extract_variable(str);
	if (var && does_var_exist(env_list, var) == 0)
	{
		if (token != NULL)
			token->is_var = true;
		if (token != NULL && token->type == HEREDOC)
			value = ft_strdup(find_env_var(env_list, var));
		else if (!token || !token->quoted)
			value = clean_spaces(find_env_var(env_list, var));
		else
			value = ft_strdup(find_env_var(env_list, var));
	}
	else if (var && ft_strcmp(var, "?") == 0)
		value = ft_itoa(g_last_exit_code);
	else
		value = ft_strdup("");
	free_str(var);
	return (value);
}
