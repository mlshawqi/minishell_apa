#include "../minishell.h"

int    pwd_cmd(t_data *data, char **args)
{
        char *cwd;

        if(args && args[0])
        {
            if(args && (ft_strlen(args[0]) > 1) && args[0][0] == '-')
            {
                print_cmd_error("minishell\npwd", "invalid option", args[0]);
                return (2);
            }
        }
        cwd = getcwd(NULL, 0);
        if (cwd)
        {
            write(1, cwd, ft_strlen(cwd));
            write(1, "\n", 1);
            if(data->pwd)
                free_str_null(&data->pwd);
            data->pwd = cwd;
        }
        else
        {
            if (data->pwd != NULL)
            {
                write(1, data->pwd, ft_strlen(data->pwd));
                write(1, "\n", 1);
            }
            else
            {
                print_cmd_error("pwd", "getcwd failed", NULL); return (1);
                return (1);
            }
        }
        return (0);
}

static int  not_valid(char *str)
{
    int     i;

    i = 0;
    if(str && str[0] == '-')
    {
        write(2, "env: invalid option -- '", 25);
        write(2, str, ft_strlen(str));
        write(2, "'\n", 2);
        g_last_exit_code = 125;
        return (125);
    }
    if(str && (str[0] == '\0' || str[0] != '\0'))
    {
        write(2, "env: ‘", 8);
        write(2, str, ft_strlen(str));
        write(2, "’: No such file or directory\n", 31);
        g_last_exit_code = 127;
        return (127);
    }
}

int    env_cmd(t_env *lst, char **arg)
{
        if(arg && arg[0])
        {
            not_valid(arg[0]);
            return (g_last_exit_code);
        }
        while(lst != NULL)
        {
            if(lst->name && lst->value && (ft_strcmp(lst->name, "_") != 0))
            {
                write(1, lst->name, ft_strlen(lst->name));
                write(1, "=", 1);
                write(1, lst->value, ft_strlen(lst->value));
                write(1, "\n", 1);
                // printf("%s=%s\n", lst->name, lst->value);
            }
            lst = lst->next;
        }
        write(1, "_=/usr/bin/env\n", 15);
        return (0);
}
