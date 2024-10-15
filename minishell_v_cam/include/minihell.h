/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 13:42:06 by cgorin            #+#    #+#             */
/*   Updated: 2024/10/16 00:27:52 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIHELL_H
# define MINIHELL_H

#include <stdlib.h> 
#include <stdio.h> 
#include <readline/readline.h> 
#include <readline/history.h> 
#include "../libft/includes/libft.h"
#include "../pipex/includes/pipex.h"
#include <pwd.h>
#include <time.h>
#include <string.h>
// Définir la couleur du texte
#define RED "\001\033[1;31m\002"
#define YELLOW "\001\033[1;33m\002"
#define GREEN "\001\033[1;32m\002"
#define RESET "\001\033[0m\002"

/* STRUCTURES */

typedef struct s_cmd
{
	char			*cmd;			// Commande entiere
	char			**args;			// Tableau d'arguments
	char			*token;
	char			*out_file;		// Fichier pour la sortie redirigée
	char			*in_file;		// Fichier pour l'entrée redirigée
	struct s_cmd	*next;			// Pointeur vers la commande suivante
}	t_cmd;

typedef struct s_env
{
	char			*key;		// Nom de la clé
	char			*value;		// Valeur
	struct s_env	*next;		// Pointeur vers la clé suivante
}	t_env;

typedef enum e_bool
{
	FALSE = 0,
	TRUE = 1
}	t_bool;

/* FUNCTIONS *//* MAIN */
void	ft_command(char *line, t_env *env);
void	ft_exec(t_cmd *cmd, t_env *env);

/* FUNCTIONS *//* BULTINS */
void	ft_exit(t_cmd *cmd);
void	ft_echo(t_cmd *cmd);
void	ft_env(t_cmd *cmd, t_env *env);
void	ft_pwd();
void	ft_cd(t_cmd *cmd, t_env *env);

/* FUNCTIONS *//* CHECK_ERRORS */
t_bool	count_redir(const char *line);
t_bool	match_quotes(char *line);

/* FUNCTIONS *//* ENVIRONMENT */
t_env	*create_env_node(char *key, char *value);
t_env	*get_env(char **env, t_env *new_env);


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

#endif