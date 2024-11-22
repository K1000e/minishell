/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 13:42:06 by cgorin            #+#    #+#             */
/*   Updated: 2024/11/15 18:08:05 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIHELL_H
# define MINIHELL_H

#include <stdlib.h> 
#include <stdio.h> 
#include <readline/readline.h> 
#include <readline/history.h> 
#include "../libft/includes/libft.h"
#include <signal.h>
#include "../srcs/pipex.h"
#include <pwd.h>
#include <time.h>
#include <string.h>

// Définir la couleur du texte
#define RED "\001\033[1;31m\002"
#define YELLOW "\001\033[1;33m\002"
#define GREEN "\001\033[1;32m\002"
#define RESET "\001\033[0m\002"

/* STRUCTURES */

typedef enum e_bool
{
	FALSE = 0,
	TRUE = 1
}	t_bool;

typedef struct s_cmd
{
	char			*cmd;			// Commande entiere
	char			**args;			// Tableau d'arguments
	char			*token;
	int				nb_token;
	int				nb_outfile;		// Nombre de redirection out
	int				nb_infile;		// Nombre de redirection in
	char			**out_file;		// Fichier pour la sortie redirigée
	char			**in_file;		// Fichier pour l'entrée redirigée
	struct s_cmd	*next;			// Pointeur vers la commande suivante
	int				*append;			// Pour savoir si on doit append ou trunk
	t_bool 			heredoc;
	t_bool			redirection;
	char			*heredoc_exit;
	t_bool			is_pipe;
	t_bool			is_bultins;
	t_bool			functional;
}	t_cmd;

typedef struct s_env
{
	char			*key;		// Nom de la clé
	char			*value;		// Valeur
	struct s_env	*next;		// Pointeur vers la clé suivante
}	t_env;

typedef struct s_pipex
{
	//pid_t	*pid;
	int		pipe_fd[2];
	int		file_i;
	int		file_o;
	int		status;
	char	*limiter;
	char	*outfile;
}	t_pipex;

typedef struct s_parse
{
	char *token_line;
	char *command_line;
}	t_parse;

extern int g_exit_code;

/* FUNCTIONS *//* MAIN */
void	ft_command(char *line, t_env *env);
void	ft_exec(t_cmd *cmd, t_env *env);

void ft_update_key(t_env *env, char *key, char *value);

/* FUNCTIONS *//* BULTINS */
void	ft_exit(t_cmd *cmd);
void	ft_echo(t_cmd *cmd);
void	ft_env(t_cmd *cmd, t_env *env);
void	ft_pwd(t_env *env);
void	ft_cd(t_cmd *cmd, t_env *env);
void	ft_unset(t_cmd *cmd, t_env *env);

/* FUNCTIONS *//* CHECK_ERRORS */
t_bool check_error(char *line);
t_bool	count_redir(const char *line);
t_bool	match_quotes(char *line);

/* FUNCTIONS *//* ENVIRONMENT */
t_env	*create_env_node(char *key, char *value);
t_env	*get_env(char **env, t_env *new_env);
void	ft_env_add_back_(t_env **lst, t_env *new);
void	ft_export(t_cmd *cmd, t_env *env);

/* FUNCTIONS *//* PARSING */
char	*ft_strndup(char *str, size_t len);
void	free_cmd_list(t_cmd *cmd_list);
t_cmd	*create_cmd_node(char *cmd_str, char *cmd_tokens);
t_cmd	*parse_cmd(char *all);
int		count_tokens(const char *cmd_tokens);
char	**tokenise_command(char *cmd_str, char *cmd_tokens);
char	check_all_char(const char cmd);

/* FUNCTIONS *//* PROMPT */
char	*get_current_path();
char	*get_username();
char	*prompt_hell(int i);

//t_list	*create_cmd_list(char *all, char *tmp, int i, int j);

void	find_executable(t_cmd *command, t_env *env);
t_env	*ft_find_key(t_env *env, char *key);

int		ft_pipex_start(t_cmd *cmd, t_env *env);
void	init_ft_pipex_start(t_pipex *pipex, t_cmd *cmd);
void	ft_ft_pipex_start(t_cmd *cmd, t_env *env, t_pipex *pipex);
void	pipeline(t_cmd *cmd, t_env *env, t_pipex *pipex, int i);

void	open_infile(t_pipex *pipex);
void	open_outfile(t_pipex *pipex);
void	free_all(t_pipex *pipex);
void	error(t_pipex *pipex, char *message, int error_code);

//pipex_execute.c
void	exec_pipe(t_pipex *pipex, t_cmd *cmd, t_env *env);
char	*get_path_variable(t_env *env);
t_bool is_valid_command_format(const char *cmd);
void execute_builtin(t_cmd *cmd, t_env *env);

void	execute_builtin_redirection(t_cmd *cmd, t_env *env);
void	parse_exec(t_cmd *cmd, t_env *env);
void	execute_command(t_cmd *cmd ,t_env *env);

t_cmd *parse_command(char *line);
t_bool check_pipe_validity(char *line, int i);
t_cmd * create_commands(t_parse *parse, int start, int end, t_cmd *list_commands);
t_cmd *	create_cmd_node_(char *cmd_str, char *cmd_tokens, t_cmd *cmd);
char **make_argument(char *cmd_str, char *cmd_tokens, t_cmd *cmd);
int count_tokens_(const char *cmd_tokens);
char **parse_args(char *cmd_str, char *cmd_tokens, char **args);
void check_char(t_parse *cmd);
t_cmd *	handle_redirection_(t_cmd *new_cmd);
void	ft_cmd_add_back(t_cmd **lst, t_cmd *new);
int	handle_output_redirection(t_cmd *cmd, char **args, int i, int *out);
int	handle_input_redirection(t_cmd *cmd, char **args, int i, int *in);
void	single_command(t_cmd *cmd ,t_env *env);
char	**base_env(t_env *env);

#endif