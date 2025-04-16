#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
# include <sys/types.h>
#include "./print/ft_printf.h"
#include "./libft/libft.h"

#define SUCCESS 0
#define FAILURE 1

extern int g_last_exit_code;

typedef struct s_separation
{	
	char			*str;
	char			*str_copy;
	bool			is_var;
	int				type;
	int				status;
	bool			join;
	struct s_separation	*prev;
	struct s_separation	*next;
}	t_separation;

typedef struct s_in_out_fds
{
	char	*infile;
	char	*outfile;
	int		fd_in;
	int		fd_out;
	int		stdin_backup;
	int		stdout_backup;
}	t_in_out_fds;

typedef struct s_cmd
{
	char				*command;
	char				**args;
	bool				pipe_output;
	int					*pipe_fd;
	t_in_out_fds			*io_fds;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct s_data
{
	bool		interactive;
	t_separation		*token;
	char		*user_input;
	char		**env;
	char		*working_dir;
	t_cmd	*cmd;
}	t_data;

enum e_token_types {
	SPACES = 1,
	WORD,
	VAR,
	PIPE,
	INPUT,
	TRUNC,
	HEREDOC,
	APPEND,
	END
};

enum e_quoting_status {
	DFLT,
	SQUOTE,
	DQUOTE
};
//util1.c
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s);
void	ft_putendl_fd(char *s, int fd);
void	ft_putstr_fd(char *s, int fd);
char	*ft_strjoin(char const *s1, char const *s2);
void	free_str(void *str);

//test1.c
t_separation	*ft_new_token(char *str, char *str_backup, int type, int status);
void	add_token(t_separation **lst, t_separation *new_token);
int	malloc_separator(t_separation **token_lst, char *str, int index, int type);
int	malloc_word(t_separation **token_lst, char *str, int index, int start);
int	is_separator(char *str, int i);
int	find_status(int status, char *str, int i);
int	malloc_word_separator(int *i, char *str, int start, t_data *data);
int	tokenization(t_data *data, char *str);

//// print_errors
char *concatenate_strings(char *base, const char *suffix);
void display_error_message(char *error_text, const char *info, int use_quotes);

#endif