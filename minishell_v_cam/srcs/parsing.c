#include "minihell.h"

/* 
** Fonction : ft_strndup
** Crée une copie d'une chaîne jusqu'à une longueur donnée.
*/

char	*ft_strndup(char *str, size_t len)
{
	char	*dup;
	size_t	len_dup;

	//len_dup = 0;
	len_dup = ft_strlen(str);
	if (len_dup > len)
		len_dup = len;
	dup = malloc((len_dup + 1) * sizeof(char));
	if (!dup)
		return NULL;
	ft_strlcpy(dup, str, len_dup + 1);
	dup[len_dup] = '\0';
	return (dup);
}

/* 
** Fonction : free_cmd_list
** Libère la mémoire allouée à la liste de commandes.
*/


void free_cmd_list(t_cmd *cmd_list) {
    t_cmd *current;
    while (cmd_list) {
        current = cmd_list;
        cmd_list = cmd_list->next;
        printf("Freeing command: %s\n", current->cmd); // Debug deallocation
        free(current->cmd);
        if (current->args) {
            for (int i = 0; current->args[i]; i++) {
                printf("Freeing argument[%d]: %s\n", i, current->args[i]);
                free(current->args[i]);
            }
            free(current->args);
        }
        free(current->out_file);
        free(current->in_file);
        free(current);
    }
}


/* 
** Fonction : create_cmd_node
** Crée un nœud avec une commande et ses tokens.
*/
t_cmd *create_cmd_node(char *cmd_str, char *cmd_tokens) {
    t_cmd *new_cmd = malloc(sizeof(t_cmd));
    if (!new_cmd) {
        fprintf(stderr, "[31m[31mMemory allocation for new_cmd failed[0m[0m\n");
        return NULL;
    }

    new_cmd->cmd = ft_strdup(cmd_str);
    if (!new_cmd->cmd) {
        fprintf(stderr, "[31m[31mMemory allocation for cmd failed[0m[0m\n");
        free(new_cmd);
        return NULL;
    }

    new_cmd->args = tokenise_command(cmd_str, cmd_tokens);
    if (!new_cmd->args) {
        fprintf(stderr, "[31m[31mFailed to tokenize command arguments[0m[0m\n");
        free(new_cmd->cmd);
        free(new_cmd);
        return NULL;
    }

    new_cmd->out_file = NULL;
    new_cmd->in_file = NULL;
    new_cmd->next = NULL;
    new_cmd->append = FALSE;

    return new_cmd;
}

/*
** Fonction qui analyse une chaîne de caractères
** et retourne une liste de commandes tokenisées.
*/

t_cmd	*parse_cmd(char *all)
{
	t_cmd	*cmd_lst;
	t_cmd	*new_cmd;
	char	*tmp;
	int		i;
	int		j;
	t_bool	in_quotes;
	char	*cmd;
	char	*tokens;
	//t_cmd *redir_cmd;

	cmd_lst = NULL;
	in_quotes = FALSE;
	tmp = ft_strdup(all);
	if (!tmp)
		return NULL;
	i = -1;
	while (tmp[++i])
		tmp[i] = check_all_char(tmp[i]);
	i = 0;
	j = 0;
	while (tmp[i])
	{
		printf("Character at tmp[%d]: %c\n", i, tmp[i]);
		if (tmp[i] == 'q')
		{
			in_quotes = !in_quotes;
			i++;
			continue;
		}
		if (!in_quotes  && (tmp[i] == '>' || tmp[i] == '<' 
			|| tmp[i] == 'p' || !tmp[i]))
		{
			if (i > j)
			{
				cmd = ft_strndup(&all[j], i - j - 1);
				tokens = ft_strndup(&tmp[j], i - j - 1);
				printf("Creating node: %s -> %s\n", cmd, tokens);
				new_cmd = create_cmd_node(cmd, tokens);
				ft_lstadd_back((t_list **)&cmd_lst, (t_list *)new_cmd);
				free(cmd);
				free(tokens);
			}
			// Créer une commande spécifique pour la redirection ou le pipe
			cmd = ft_strndup(&all[i], 1);
			tokens = ft_strndup(&tmp[i], 1);
			printf("Creating redir: %s -> %s\n", cmd, tokens);
			t_cmd *redir_cmd = create_cmd_node(cmd, tokens);
			ft_lstadd_back((t_list **)&cmd_lst, (t_list *)redir_cmd);
			free(cmd);
			free(tokens);
			i++;
			j = i;
			continue;
		}
		i++;
	}
	// Ajouter la dernière commande si elle existe
	if (j < i)
	{
		cmd = ft_strndup(&all[j], i - j);
		tokens =  ft_strndup(&tmp[j], i - j);
		printf("Adding final command: %s -> %s\n", cmd, tokens);
		new_cmd = create_cmd_node(cmd, tokens);
		ft_lstadd_back((t_list **)&cmd_lst, (t_list *)new_cmd);
		free(cmd);
		free(tokens);
	}
	free(tmp);
	return cmd_lst;
}


/*
** count_tokens compte le nombre de tokens dans une chaîne 
** en fonction de types de caractères spécifiques.
*/

int count_tokens(const char *cmd_tokens)
{
	int count;
	int i;

	count = 0;
	i = 0;
	while (cmd_tokens[i])
	{
		if (cmd_tokens[i] == 'q')
		{
			count++;
			while (cmd_tokens[++i] && !(cmd_tokens[i] == 'q'))
				;
		}
		else if (cmd_tokens[i] == 'c')
		{
			// Chaque caractère de type 'c' est un token unique
			count++;
			while (cmd_tokens[i] && cmd_tokens[i] == 'c')
				i++;
		}
		else if (cmd_tokens[i] == 'p')
		{
			//Chaque caractère de type 'p', '>', '<', ou 'a' est un token unique
			count++;
			i++;
		}
		else if (cmd_tokens[i] == '>' || cmd_tokens[i] == '<')
		{
			if ((cmd_tokens[i] == '>' && cmd_tokens[i + 1] == '>' ) 
				|| (cmd_tokens[i] == '<' && cmd_tokens[i + 1] == '<'))
				i += 2;
			else // Token unique pour redirection simple
				i++;
			count++;
		}
		else
			i++;
	}
	return count;
}

/*
** tokenise_command divise la chaîne de commande en tokens 
** selon les types 'c', 'q', 'p', '>', '<', et 'a'.
** reprend un peu la logique du schema parsing (voir notion)
*/

char **tokenise_command(char *cmd_str, char *cmd_tokens) {
    int i = 0, j;
    int token_count = 0;
    int total_tokens = count_tokens(cmd_tokens); // Calculate the total number of tokens
    char **tokens = (char **)malloc(sizeof(char *) * (total_tokens + 1)); // +1 for NULL at the end

    if (!tokens) {
        fprintf(stderr, "[31m[31mMemory allocation for tokens failed[0m[0m\n");
        return NULL;
    }

    // [31m[31mInitialize all elements to NULL to avoid undefined behavior[0m[0m
    for (int k = 0; k <= total_tokens; k++) {
        tokens[k] = NULL;
    }

    while (cmd_tokens[i] && token_count < total_tokens) {
        while (cmd_str[i] == ' ' && cmd_tokens[i]) {
            i++;
        }
        if (!cmd_tokens[i])
            break;

        // Command token handling
        if (cmd_tokens[i] == 'c') {
            j = i;
            while (cmd_tokens[j] && cmd_tokens[j] == 'c') {
                j++;
            }
            tokens[token_count] = ft_strndup(&cmd_str[i], j - i);
            if (!tokens[token_count]) {
                fprintf(stderr, "[31mMemory allocation for token failed[0m\n");
                // [31m[31mFree any already allocated tokens and return NULL to avoid leaks[0m[0m
                for (int k = 0; k < token_count; k++) {
                    free(tokens[k]);
                }
                free(tokens);
                return NULL;
            }
            token_count++;
            i = j;
        } else if (cmd_tokens[i] == 'p' || cmd_tokens[i] == '>' || cmd_tokens[i] == '<') {
            tokens[token_count] = ft_strndup(&cmd_str[i], 1);
            if (!tokens[token_count]) {
                fprintf(stderr, "[31mMemory allocation for token failed[0m\n");
                for (int k = 0; k < token_count; k++) {
                    free(tokens[k]);
                }
                free(tokens);
                return NULL;
            }
            token_count++;
            i++;
        } else {
            i++;
        }
    }

    tokens[token_count] = NULL; // [31m[31mNull-terminate the array[0m[0m
    return tokens;
}
/*
** check_all_char assigne un type de token à chaque caractère 
** et renvoie le type.
*/

char check_all_char(const char cmd)
{
	if (cmd == ';') // Separateur
		return 's';
	if (cmd == '|') // Pipe
		return 'p';
	else if (cmd == '>') // Redirection (a voir comment on fait pour >>)
		return '>';
	else if (cmd == '<') // Redirection (a voir comment on fait pour <<)
		return '<';
	else if (cmd == '"')
		return 'q'; // Double quotes " "
	else if (cmd == '\'')
		return 'q'; // Single quotes ' '
	else if (cmd == '$')
		return 'a'; // Arguments $ARG pas bien traité ... !!
	else if (!ft_isspace(cmd))
		return 'c';  // Command
	else 
		return ' '; // Pour les autres
}
