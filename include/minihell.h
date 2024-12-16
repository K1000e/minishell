/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 13:42:06 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/16 23:44:03 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIHELL_H
# define MINIHELL_H

# include <stdlib.h> 
# include <stdio.h> 
# include <readline/readline.h> 
# include <readline/history.h> 
# include "../libft/includes/libft.h"
# include <signal.h>
# include <pwd.h>
# include <time.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <fcntl.h>
# include <errno.h>

// Définir la couleur du texte
# define RESET "\001\033[0m\002"
# define R "\001\033[1;31m\002"
# define G "\001\033[1;32m\002"
# define Y "\001\033[1;33m\002"
# define B "\001\033[1;34m\002"
# define P "\001\033[1;35m\002"
# define C "\001\033[1;36m\002"

# define ERR_SYNTAX "bash: syntax error near unexpected token "
/* STRUCTURES */

typedef enum e_bool
{
	FALSE = 0,
	TRUE = 1
}	t_bool;

typedef struct s_cmd
{
	struct s_cmd	*next;
	char			*cmd;
	char			*token;
	char			**args;
	char			**args_t;
	char			*order_file;
	char			*heredoc;
	char			**out_file;
	char			**in_file;
	char			**heredoc_delimiter;
	int				nb_token;
	int				nb_outfile;
	int				nb_infile;
	int				nb_heredoc;
	int				*append;
	int				pid;
	t_bool			redirection;
	t_bool			heredoc_redirection;
	t_bool			is_pipe;
	t_bool			is_bultins;
	t_bool			path;
}	t_cmd;

typedef struct s_env
{
	char			*executable;
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_pipex
{
	int		pipe_fd[2];
	int		file_i;
	int		file_o;
	int		heredoc_fd;
	char	*limiter;
	char	*outfile;
	char	*heredoc_file;
}	t_pipex;

typedef struct s_parse
{
	char	*token_line;
	char	*cmd_line;
}	t_parse;

typedef struct s_file
{
	int		j;
	int		k;
	int		h;
	char	type;
}	t_file;

extern int	g_exit_code;

/* FUNCTIONS 			 ENVIRONMENT 					*/
t_env	*create_env(char **env, t_env *new_env, char *executable, int i);
char	**base_env(t_env *env);
t_env	*create_env_node(char *key, char *value);
void	ft_env_add_back(t_env **lst, t_env *new);

/* FUCTIONS 			 SIGNALS 						*/
void	set_signal_action(void (*handler)(int));
void	sigint_heredoc_handler(int signal);
void	sigint_handler(int signal);

/* FUNCTIONS 			 EXPANDER 						*/
char	*expand_env_vars(char *input, t_env *env);

/* FUNCTIONS 			 PARSING 						*/
t_cmd	*parse_command(char *line);
char	**clear_redir(t_cmd *c);
t_bool	check_pipe_validity(char *line, int i);
t_cmd	*create_commands(t_parse *parse, int start, int end, t_cmd *l_cmd);
void	make_argument(char *cmd_str, char *cmd_tokens, t_cmd *cmd);
t_cmd	*handle_redirection(t_cmd *new_cmd);
t_bool	is_invalid_redir(t_parse *parse, int i);
t_bool	return_error(char *error, int exit_code);
char	**add_to_tab(char **tab, const char *str);
t_bool	is_within_single_quotes(const char *input, int index);
void	ft_cmd_add_back(t_cmd **lst, t_cmd *new);
void	clear_quotes(t_parse *parse);
void	tokenize_char(t_parse *parse);
void	execute_command(t_cmd *cmd, t_env *env);

/* FUNCTIONS 			 EXECUTION 						*/
void	execute_builtin(t_cmd *cmd, t_env *env, int single);
void	exec_non_builtins(t_cmd *cmd, t_env *env);
t_bool	is_builtin(char *cmd);
int		redirection_exec(t_cmd *cmd, t_pipex *pipex,
			t_bool should_exit);
void	handle_heredoc(char *delimiter, t_pipex *pipex, t_bool is_last);
void	reopen_heredoc(t_pipex *pipex, t_bool is_last);
int		open_infile(t_pipex *pipex, t_cmd *cmd, int j);
int		open_outfile(t_pipex *pipex, t_cmd *cmd, int k);
void	find_executable(t_cmd *command, t_env *env);

/*FUCNTIONS 			 CHECK_ERRORS 					*/
t_bool	check_redir(const char *line, char token, int i);
t_bool	count_redir(const char *line);
t_bool	match_quotes(char *line);

/* FUNCTIONS 			 BULTINS 						*/
void	ft_exit(t_cmd *cmd);
void	ft_echo(t_cmd *cmd);
void	ft_pwd(t_env *env);
void	ft_env(t_cmd *cmd, t_env *env);
void	ft_cd(t_cmd *cmd, t_env *env);
void	ft_unset(t_cmd *cmd, t_env *env);
void	ft_export(t_cmd *cmd, t_env *env);
t_env	*ft_find_key(t_env *env, char *key);
void	ft_update_key(t_env *env, char *key, char *value);
void	ft_sort_env(t_env **env);

/* FUNCTIONS 			 UTILS 							*/
char	*ft_strjoin_free(char *s1, char *s2, int is_free);
void	is_pipe(t_cmd *cmd);

/* FUNCTIONS 			 FREE 							*/
void	free_all(t_pipex *pipex);
void	free_string_array(char **array);
void	free_parse(t_parse *parse);
void	free_cmd_list(t_cmd *cmd_list);

/* FUNCTIONS 			 EMOTES 						*/
void	ft_kitty(void);
void	print_chill_guy(void);

#endif