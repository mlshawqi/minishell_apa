#include "../minishell.h"

int     is_builtin(t_data *data, char *command, char **args)
{
        if(ft_strcmp(command, "pwd") == 0)
                g_last_exit_code = pwd_cmd(args + 1);
        else if(ft_strcmp(command, "echo") == 0)
                g_last_exit_code = echo_cmd(args + 1);
        else if(ft_strcmp(command, "cd") == 0)
                g_last_exit_code = cd_cmd(args + 1, &data->env);
        else if(ft_strcmp(command, "env") == 0)
                g_last_exit_code = env_cmd(data->env, args + 1);
        else if(ft_strcmp(command, "exit") == 0)
                g_last_exit_code = exit_cmd(args + 1);
        else if(ft_strcmp(command, "export") == 0)
                g_last_exit_code = export_cmd(&data->env, &data->export, args + 1);
        else if(ft_strcmp(command, "unset") == 0)
                g_last_exit_code = unset_cmd(&data->env, &data->export, args + 1);
}

void    execution(t_data *data)
{
        // t_cmd   *tmp;
        // int     i;

        // // printf("%d\n", g_last_exit_code);
        // tmp = data->cmd;
        // while(tmp)
        // {
        //         is_builtin(data, tmp->command, tmp->args);
        //         tmp = tmp->next;
        // }
        int i = 0;
        while(data->cmd->args[i])
        {
                printf("arg [%d] %s\n",i,  data->cmd->args[i]);
                i++;
        }
}






















// cc main.c -lreadline
// int g_last_exit_code = 0;

// void    handle_sigint(int sig)
// {
//         (void)sig;
        // rl_redisplay();
        // printf("\nMinishell~$ ");
// }
// void    manage_exenv(t_data *data, char **env)
// {
//         data->envrmnt = NULL;
//         data->export= NULL;
//         copy_env(env, &data->envrmnt);
//         copy_env(env, &data->export);
// }

// int main(int argc, char **argv, char **env)
// {
//         char *input;
//         (void)argc;
//         (void)argv;
//         t_data  data;

//         manage_exenv(&data, env);
//         signal(SIGINT, handle_sigint);
//         while (1)
//         {
//                 input = readline("Minishell~$ ");
//                 if (input == NULL)
//                 {
//                     printf("exit\n");
//                     free(input);
//                     break;
//                 }
//                 if (*input) {
//                     add_history(input);
//                 }
//                 if(ft_strcmp(input, "mkdir") == 0)
//                 {
//                         char *args[] = {"mkdir", "folder", NULL};
//                         if (ft_execve(data.envrmnt, input, args) == -1)
//                                 break;
//                 }
//                 else
//                 {
//                         char *args[] = {"ls", NULL};
//                         if (ft_execve(data.envrmnt, input, args) == -1)
//                                 break;
//                 }
// ;
//                 free(input);
//         }
//         rl_clear_history();
//         ft_lstclear(&data.export);
//         ft_lstclear(&data.envrmnt);
//         return 0;
// }


