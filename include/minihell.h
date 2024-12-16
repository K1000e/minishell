/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 13:42:06 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/16 13:27:21 by codespace        ###   ########.fr       */
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
	char			*cmd;
	char			**args;
	char			**args_t;
	char			*token;
	int				nb_token;
	int				nb_outfile;
	int				nb_infile;
	char			*order_file;
	char			**out_file;
	char			**in_file;
	struct s_cmd	*next;
	int				*append;
	char			*heredoc;
	t_bool			redirection;
	t_bool			heredoc_redirection;
	char			**heredoc_delimiter;
	int				nb_heredoc;
	t_bool			is_pipe;
	t_bool			is_bultins;
	t_bool			path;
	int				pid;
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
	char	*limiter;
	char	*outfile;
	char	*heredoc_file;
	int		heredoc_fd;
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

/* FUNCTIONS *//* MAIN */
void	ft_command(char *line, t_env *env);

void	ft_update_key(t_env *env, char *key, char *value);

/* FUNCTIONS *//* BULTINS */
void	ft_exit(t_cmd *cmd);
void	ft_echo(t_cmd *cmd);
void	ft_env(t_cmd *cmd, t_env *env);
void	ft_pwd(t_env *env);
void	ft_cd(t_cmd *cmd, t_env *env);
void	ft_unset(t_cmd *cmd, t_env *env);

/* FUNCTIONS *//* CHECK_ERRORS */
t_bool	count_redir(const char *line);
t_bool	match_quotes(char *line);

/* FUNCTIONS *//* ENVIRONMENT const*/
t_env	*create_env_node(char *key, char *value);
t_env	*create_env(char **env, t_env *new_env, char *executable, int i);
void	ft_env_add_back_(t_env **lst, t_env *new);
void	ft_export(t_cmd *cmd, t_env *env);
void	ft_sort_env(t_env **env);
void	ft_print_declare_env(t_env *env);
t_bool	check_validity_export(const char *key);

/* FUNCTIONS *//* PARSING */
//char	*ft_strndup(char *str, size_t len);
void	free_cmd_list(t_cmd *cmd_list);

/* FUNCTIONS *//* EXECUTION */

void	find_executable(t_cmd *command, t_env *env);
t_env	*ft_find_key(t_env *env, char *key);
char	*get_path_variable(t_env *env);
void	execute_builtin(t_cmd *cmd, t_env *env, int single);

void	execute_builtin_redirection(t_cmd *cmd, t_env *env);
void	execute_command(t_cmd *cmd, t_env *env);

t_cmd	*parse_command(char *line);
t_bool	check_pipe_validity(char *line, int i);
t_cmd	*create_commands(t_parse *parse, int start, int end,
			t_cmd *list_commands);
t_cmd	*create_cmd_node_(char *cmd_str, char *cmd_tokens, t_cmd *cmd);
void	make_argument(char *cmd_str, char *cmd_tokens, t_cmd *cmd);

int		count_tokens_(const char *cmd_tokens);
void	parse_args(char *cmd_str, char *cmd_tokens, t_cmd *cmd);
void	check_char(t_parse *cmd);
t_cmd	*handle_redirection_(t_cmd *new_cmd);
void	ft_cmd_add_back(t_cmd **lst, t_cmd *new);
char	**base_env(t_env *env);

char	*ft_strjoin_free(char *s1, char *s2, int is_free);
t_bool	is_builtin(char *cmd);
int		redirection_exec_builtins(t_cmd *cmd, t_pipex *pipex,
			t_bool should_exit);

void	set_signal_action(void (*handler)(int));
void	sigint_heredoc_handler(int signal);
void	sigint_handler(int signal);

char	*expand_env_vars(char *input, t_env *env);
char	**add_to_tab(char **tab, const char *str);
t_bool	is_directory(const char *path);

void	ft_kitty(void);
void	print_chill_guy(void);
t_bool	check_redir(const char *line, char token, int i);

int		open_infile(t_pipex *pipex, t_cmd *cmd, int j);
int		open_outfile(t_pipex *pipex, t_cmd *cmd, int k);

void	handle_heredoc(char *delimiter, t_pipex *pipex, t_bool is_last);

void	reopen_heredoc(t_pipex *pipex, t_bool is_last);
char	**clear_redir(t_cmd *cmd);
int		count_redirection(char *cmd, char type);

void	clear_quotes(t_parse *parse);
void	free_cmd_list(t_cmd *cmd_list);
void	free_string_array(char **array);
t_bool	return_error(char *error, int exit_code);
void	is_pipe(t_cmd *cmd);

int		ft_envsize(t_env *lst);
void	free_all(t_pipex *pipex);
void	exec_non_builtins(t_cmd *cmd, t_env *env);
void	free_parse(t_parse *parse);
t_bool	is_within_single_quotes(const char *input, int index);
t_bool	is_invalid_redir(t_parse *parse, int i);
#endif