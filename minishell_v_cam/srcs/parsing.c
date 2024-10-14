#include "minihell.h"

/*
** ft_strndup crée une copie d'une chaîne jusqu'à une longueur donnée.
*/

char	*ft_strndup(char *str, size_t len)
{
	char	*dup;
	size_t	len_dup;

	len_dup = 0;
	len_dup = ft_strlen(str);
	if (len_dup > len)
		len_dup = len;
	dup = malloc((len_dup + 1) * sizeof(char));
	if (!dup)
		return NULL;
	ft_strlcpy(dup, str, len_dup + 1);
	return dup;
}


/* Fonction pour libérer la mémoire allouée à la liste de commandes */
void free_cmd_list(t_cmd *cmd_list)
{
	t_cmd *current;
		
	while (cmd_list)
	{
		current = cmd_list;
		cmd_list = cmd_list->next;
		free(current->cmd);
		if (current->args) {
			for (int i = 0; current->args[i]; i++)
				free(current->args[i]);
			free(current->args);
		}
		free(current->out_file);
		free(current->in_file);
		free(current);
	}
}

/*
** create_cmd_node crée un nœud avec une commande et ses tokens.
*/

t_cmd *create_cmd_node(char *cmd_str, char *cmd_tokens)
{
	t_cmd *new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd) {
		return NULL;
	}
	new_cmd->cmd = ft_strdup(cmd_str);
	new_cmd->args = tokenise_command(cmd_str, cmd_tokens);
	new_cmd->out_file = NULL;
	new_cmd->in_file = NULL;
	new_cmd->next = NULL;
	return (new_cmd);
}

/*
** parse_cmd divise la chaîne en commandes séparées par ';' 
** et retourne une liste de commandes tokenisées.
*/

t_cmd	*parse_cmd(char *all)
{
	t_cmd	*cmd_lst;
	char	*tmp;
	int		i;
	int		j;
	t_bool	in_quotes;

	cmd_lst = NULL;
	in_quotes = FALSE;
	tmp = ft_strdup(all);
	i = -1;
	while (tmp[++i])
		tmp[i] = check_all_char(tmp[i]);
	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == 'q')
		{
			in_quotes = !in_quotes;
			i++;
			continue;
		}
		if (!in_quotes  && (tmp[i] == '>' || tmp[i] == '|' || !tmp[i]))
		{
			if (i > j)
			{ //penser a free les strndup !!!!!!!!!!!!!!!!!!!!!!!!!!!!! segfault + leaks sa mere
				t_cmd *new_cmd = create_cmd_node(ft_strndup(&all[j], i - j), ft_strndup(&tmp[j], i - j));
				ft_lstadd_back((t_list **)&cmd_lst, (t_list *)new_cmd);
			}
			// Créer une commande spécifique pour la redirection ou le pipe
			t_cmd *redir_cmd = create_cmd_node(ft_strndup(&all[i], 1), ft_strndup(&tmp[i], 1));
			ft_lstadd_back((t_list **)&cmd_lst, (t_list *)redir_cmd);
			i++;
			j = i;
			continue;
		}
		i++;
	}
	// Ajouter la dernière commande si elle existe
	if (j < i) {
		t_cmd *new_cmd = create_cmd_node(ft_strndup(&all[j], i - j), ft_strndup(&tmp[j], i - j));
		ft_lstadd_back((t_list **)&cmd_lst, (t_list *)new_cmd);
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
	i = -1;
	while (cmd_tokens[++i])
	{
		if (cmd_tokens[i] == 'q')
		{
			count++;
			while (cmd_tokens[i] && !(cmd_tokens[i] == 'q'))
				i++;
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
			if ((cmd_tokens[i] == '>' && cmd_tokens[i + 1] == '>' ) || (cmd_tokens[i] == '<' && cmd_tokens[i + 1] == '<'))
			{
				// Token unique pour redirection heredoc
				count++;
				i += 2;
			}
			else
			{
				// Token unique pour redirection simple
				count++;
				i++;
			}
		}
	}
	return count;
}

/*
** tokenise_command divise la chaîne de commande en tokens 
** selon les types 'c', 'q', 'p', '>', '<', et 'a'.
** reprend un peu la logique du schema parsing (voir notion)
*/

char **tokenise_command(char *cmd_str, char *cmd_tokens)
{
	int i = 0, j;
	int token_count = 0;
	char **tokens;

	tokens = (char **)malloc(sizeof(char *) * (count_tokens(cmd_tokens) + 1));
	while (cmd_tokens[i])
	{
		// Ignorer les espaces dans la chaîne de commande
		while (cmd_str[i] == ' ' && cmd_tokens[i])
			i++;
		if (!cmd_tokens[i])
			break;
		 if (cmd_tokens[i] == 'q')
		{
			// Token quoted
			j = i + 1;
			while (cmd_tokens[++j] && cmd_tokens[j] != 'q')
				j++;
			tokens[token_count++] = ft_strndup(&cmd_str[i], j + 1 - i);
			i = j;
		}
		if (cmd_tokens[i] == 'c')
		{
			// Token command ou quoted
			j = i;
			while (cmd_tokens[j] && cmd_tokens[j] == 'c')
				j++;
			tokens[token_count++] = ft_strndup(&cmd_str[i], j - i);
			i = j;
		}
		else if (cmd_tokens[i] == 'p')
		{
			// Token unique pour pipe ou redirection
			tokens[token_count++] = ft_strndup(&cmd_str[i], 1);
			i++;
		}
		else if (cmd_tokens[i] == '>' || cmd_tokens[i] == '<')
		{
			if (cmd_tokens[i] == '>' && cmd_tokens[i + 1] == '>')
			{
				// Token unique pour redirection append
				tokens[token_count++] = ft_strndup(&cmd_str[i], 2);
				i++;
			}
			else if (cmd_tokens[i] == '<' && cmd_tokens[i + 1] == '<')
			{
				// Token unique pour redirection heredoc
				tokens[token_count++] = ft_strndup(&cmd_str[i], 2);
				i++;
			}
			else // Token unique pour redirection standard
				tokens[token_count++] = ft_strndup(&cmd_str[i], 1);
			i++;
		}
		else if (cmd_tokens[i] == 'a')
		{
		// ça va pas ici je sais pas trop comment traité les args ($ARG=)
			/* j = i;
			while (cmd_tokens[j] == 'a' || cmd_str[j] == '$')
				j++;
			tokens[token_count++] = ft_strndup(&cmd_str[i], j - i);
			i = j; */
		}
		else
		{
			i++;
		}
	}
	tokens[token_count] = NULL;
	return tokens;
}

/*
** check_all_char assigne un type de token à chaque caractère 
** et retourne le type.
*/

char check_all_char(const char cmd)
{
	/* if (cmd == ';') // Separateur
		return 's'; */
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
	else if ((cmd >= '0' && cmd <= '9') || (cmd >= 'a' && cmd <= 'z')
		|| (cmd >= 'A' && cmd <= 'Z') /* || (cmd == ' ') */)
		return 'c'; // Command
	else 
		return 'o'; // Pour les autres
}
