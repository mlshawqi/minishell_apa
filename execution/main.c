#include "minishell.h"

// cc main.c -lreadline

// void    handle_sigint(int sig)
// {
//     rl_redisplay();
//     printf("\nMinishell~$ ");
// }

int main(int argc, char **argv, char **env)
{
        // char *input;
        (void)argc;
        (void)argv;
        t_env   *envrmnt;

        envrmnt = NULL;
        copy_env(env, &envrmnt, NULL,'r');
        // export_cmd(&envrmnt, NULL);
        env_cmd(envrmnt);
        printf("-------------------------------");
        unset_cmd(&envrmnt, "yy");
        env_cmd(envrmnt);
        ft_lstclear(&envrmnt);
        // signal(SIGINT, handle_sigint);
        // while (1)
        // {
        //     input = readline("Minishell~$ ");
        //     if (input == NULL || (strcmp(input, "exit") == 0))
        //     {
        //         printf("exit\n");
        //         free(input);
        //         break;
        //     }
        //     if (*input) {
        //         add_history(input);
        //     }
        //     free(input);
        // }
        // rl_clear_history();
        return 0;
}


