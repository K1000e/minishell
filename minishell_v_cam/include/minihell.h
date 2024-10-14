/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 13:42:06 by cgorin            #+#    #+#             */
/*   Updated: 2024/10/14 18:56:40 by cgorin           ###   ########.fr       */
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
#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[1;32m"
#define RESET "\033[0m"

/* STRUCTURES */

typedef struct s_cmd {
	char *cmd;			// Nom de la commande
	char **args;		// Tableau d'arguments
	char *out_file;		// Fichier pour la sortie redirigée
	char *in_file;		// Fichier pour l'entrée redirigée
	struct s_cmd *next;	// Pointeur vers la commande suivante
}	t_cmd;

typedef enum e_bool
{
	FALSE = 0,
	TRUE = 1
}	t_bool;

/* FUNCTIONS */
char *ft_strndup(char *str, size_t len);
t_list	*create_cmd_list(char *all, char *tmp, int i, int j);
t_cmd	*parse_cmd(char *all);
int count_tokens(const char *cmd_tokens);
char **tokenise_command(char *cmd_str, char *cmd_tokens);
char check_all_char(const char cmd);

void ft_exit(t_cmd *cmd);
void free_cmd_list(t_cmd *cmd_list);

#endif