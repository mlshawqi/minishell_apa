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
        t_cmd   *tmp;
        int     i;

        tmp = data->cmd;
        while(tmp)
        {
                printf("command %s\n", tmp->command);
                while(tmp->args[i])
                {
                        printf("arg[%d]  %s\n", i, tmp->args[i]);
                        i++;
                }
                if(tmp->pipe_output)
                        printf("pipe yes\n");
                if(tmp->io_fds->fd_in)
                        printf("fd_in %d\n", tmp->io_fds->fd_in);
                if(tmp->io_fds->fd_out)
                        printf("fd_out %d\n", tmp->io_fds->fd_out);
                if(tmp->io_fds->infile)
                        printf("infile %s\n", tmp->io_fds->infile);
                if(tmp->io_fds->outfile)
                        printf("otufile %s\n", tmp->io_fds->outfile);
                if(tmp->io_fds->heredoc_delimiter)
                        printf("herdoc %s\n", tmp->io_fds->heredoc_delimiter);
                tmp = tmp->next;
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


