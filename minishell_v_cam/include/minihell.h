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

typedef enum e_bool
{
	FALSE = 0,
	TRUE = 1
}	t_bool;

/* FUNCTIONS */
char *ft_strndup(char *str, size_t len);
t_list	*create_cmd_list(char *all, char *tmp, int i, int j);
t_list	*parse_cmd(char *all);
int count_tokens(const char *cmd_tokens);
char **tokenise_command(char *cmd_str, char *cmd_tokens);
char check_all_char(const char cmd);

#endif