#include "minihell.h"

/*
** ft_strndup crée une copie d'une chaîne jusqu'à une longueur donnée.
*/

char	*ft_strndup(char *str, size_t len)
{
	char	*dup;
	size_t	len_dup;

	len_dup = ft_strlen(str);
	if (len_dup > len)
		len_dup = len;
	dup = malloc((len_dup + 1) * sizeof(char));
	if (!dup)
		return NULL;
	ft_strlcpy(dup, str, len_dup + 1);
	return dup;
}

/*
** create_cmd_list crée un nœud de liste avec une commande 
** et ses tokens entre les indices i et j.
*/

t_list	*create_cmd_list(char *all, char *tmp, int i, int j)
{
	char	*cmd_str;
	char	*cmd_tokens;
	char	**cmd_split;
	t_list	*list_cmd;

	cmd_str = ft_strndup(&all[i], j - i);
	cmd_tokens = ft_strndup(&tmp[i], j - i);
	cmd_split = tokenise_command(cmd_str, cmd_tokens);
	list_cmd = ft_lstnew(cmd_split);
	free(cmd_str);
	free(cmd_tokens);
	return (list_cmd);
}

/*
** parse_cmd divise la chaîne en commandes séparées par ';' 
** et retourne une liste de commandes tokenisées.
*/

t_list	*parse_cmd(char *all)
{
	t_list	*list_cmd;
	char	*tmp;
	int		i;
	int		j;

	list_cmd = NULL;
	tmp = ft_strdup(all);
	i = -1;
	while (tmp[++i])
		tmp[i] = check_all_char(tmp[i]);
	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == 's' && ++i)
			continue;
		j = i;
		while (tmp[j] && tmp[j] != 's')
			j++;
		ft_lstadd_back(&list_cmd, create_cmd_list(all, tmp, i, j));
		i = j;
	}
	free(tmp);
	return (list_cmd);
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
		if (cmd_tokens[i] == 'c' || cmd_tokens[i] == 'q')
		{
			count++;
			while (cmd_tokens[i] && (cmd_tokens[i] == 'c' 
				|| cmd_tokens[i] == 'q'))
				i++;
		}
		else if (cmd_tokens[i] == 'p' || cmd_tokens[i] == '>' 
			|| cmd_tokens[i] == '<' || cmd_tokens[i] == 'a')
		{
			//Chaque caractère de type 'p', '>', '<', ou 'a' est un token unique
			count++;
			i++;
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
	char **tokens = (char **)malloc(sizeof(char *) * (count_tokens(cmd_tokens) + 1));

	while (cmd_tokens[i])
	{
		// Ignorer les espaces dans la chaîne de commande
		while (cmd_str[i] == ' ' && cmd_tokens[i])
			i++;
		if (!cmd_tokens[i])
			break;
		if (cmd_tokens[i] == 'c' || cmd_tokens[i] == 'q')
		{
			// Token command ou quoted
			j = i;
			while (cmd_tokens[j] && (cmd_tokens[j] == 'c' || cmd_tokens[j] == 'q') && cmd_str[j] != ' ')
				j++;
			tokens[token_count++] = ft_strndup(&cmd_str[i], j - i);
			i = j;
		}
		else if (cmd_tokens[i] == 'p' || cmd_tokens[i] == '>' || cmd_tokens[i] == '<')
		{
			// Token unique pour pipe ou redirection
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
	if (cmd == ';') // Separateur
		return 's';
	else if (cmd == '|') // Pipe
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
		|| (cmd >= 'A' && cmd <= 'Z'))
		return 'c'; // Command
	else 
		return 'o'; // Pour les autres
}
