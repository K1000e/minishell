#include "minihell.h"

/* 
** Fonction : ft_strndup
** Crée une copie d'une chaîne jusqu'à une longueur donnée.
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
	dup[len_dup] = '\0';
	return (dup);
}

/* 
** Fonction : free_cmd_list
** Libère la mémoire allouée à la liste de commandes.
*/

void free_cmd_list(t_cmd *cmd_list)
{
	t_cmd *current;
	int i;
	printf("here\n");	
	current = cmd_list;
	while (current)
	{
		if (current->cmd)
			free(current->cmd);
		if (current->args)
		{
			i = -1;
			while (current->args[++i])
				free(current->args[i]);
			free(current->args);
		}
		if (current->out_file)
			free(current->out_file);
		else if (current->out_file)
			free(current->out_file);
		current = current->next;
	}
	free(cmd_list);
}

int	handle_output_redirection(t_cmd *cmd, char **args, int i)
{
	if (args[i + 1])
	{
		cmd->out_file = ft_strdup(args[i + 1]);
		cmd->append = (ft_strcmp(args[i], ">>") == 0);
		return (i + 2);
	}
	else
	{
		printf("Error: Missing file for redirection.\n");
		free_cmd_list(cmd);
		return (-1);
	}
}

/*
** Fonction : handle_input_redirection
** Gère la redirection d'entrée.
*/

int	handle_input_redirection(t_cmd *cmd, char **args, int i)
{
	if (args[i + 1])
	{
		cmd->in_file = ft_strdup(args[i + 1]);
		return (i + 2);
	}
	else
	{
		printf("Error: Missing file for input redirection.\n");
		free_cmd_list(cmd);
		return (-1);
	}
}

t_cmd *redirection(t_cmd *cmd)
{
	int i;
	int arg_count;

	i = 0;
	arg_count = 0;
	while (cmd->args[i])
	{
		if (ft_strcmp(cmd->args[i], ">") == 0
			|| ft_strcmp(cmd->args[i], ">>") == 0)
		{
			i = handle_output_redirection(cmd, cmd->args, i);
			if (i == -1)
			    return NULL;
		}
		else if (ft_strcmp(cmd->args[i], "<") == 0)
		{
			i = handle_input_redirection(cmd, cmd->args, i);
			if (i == -1)
			    return NULL;
		}
		else
			cmd->args[arg_count++] = cmd->args[i++];
	}
	cmd->args[arg_count] = NULL;
	return (cmd);
}

/* 
** Fonction : create_cmd_node
** Crée un nœud avec une commande et ses tokens.
*/
t_cmd *create_cmd_node(char *cmd_str, char *cmd_tokens)
{
	t_cmd *new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return NULL;
	new_cmd->cmd = ft_strdup(cmd_str);
	new_cmd->args = tokenise_command(cmd_str, cmd_tokens);
	new_cmd->out_file = NULL;
	new_cmd->in_file = NULL;
	new_cmd->next = NULL;
	new_cmd->append = FALSE;
	redirection(new_cmd);
	//free(cmd_str);
	return (new_cmd);
}


void	ft_cmd_add_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*ptr;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ptr = *lst;
	while (ptr->next)
		ptr = ptr->next;
	ptr->next = new;
}

/*
** Fonction : handle_quotes
** Gère l'état de in_quotes en fonction du caractère actuel.
*/

t_bool	handle_quotes(char current, t_bool in_quotes)
{
	if (current == '"' || current == '\'')
		return (!in_quotes);
	return (in_quotes);
}

/*
** Fonction : handle_redirection
** Gère la logique de redirection des fichiers.
*/

int	handle_redirection(t_cmd *new_cmd, char *tokens, char *all, int k, int j)
{
	char	*file;

	k++;
	while (tokens[k] && tokens[k] == ' ')
		k++;
	if (tokens[k])
	{
		file = ft_strndup(&all[j + k], strlen(&all[j + k]));
		if (tokens[k - 1] == '>')
		{
			new_cmd->out_file = file;
			new_cmd->append = (tokens[k - 2] == '>' && tokens[k - 3] == '>');
			printf("Adding output redirection to: %s\n", new_cmd->out_file);
		}
		else if (tokens[k - 1] == '<')
		{
			new_cmd->in_file = file;
			printf("Adding input redirection to: %s\n", new_cmd->in_file);
		}
		k++;
	}
	else
	{
		fprintf(stderr, "Error: Missing file for redirection.\n");
		return (-1);
	}
	return (k);
}

/*
** Fonction : process_command_node
** Crée un nouveau nœud de commande et le traite.
*/

void free_cmd_node(t_cmd *cmd_node)
{
	if (cmd_node)
	{
		if (cmd_node->cmd)
			free(cmd_node->cmd);  // Free the command string
		if (cmd_node->args)
		{
			int i = 0;
			while (cmd_node->args[i])
			{
				free(cmd_node->args[i]);  // Free each argument
				i++;
			}
			free(cmd_node->args);  // Free the args array
		}
		free(cmd_node);  // Finally, free the command node itself
	}
}


t_cmd	*process_command_node(char *all, char *tmp, int i, int j)
{
	t_cmd	*new_cmd;
	char	*cmd;
	char	*tokens;
	int		k;

	cmd = ft_strndup(&all[j], i - j);
	tokens = ft_strndup(&tmp[j], i - j);
	new_cmd = create_cmd_node(cmd, tokens);
	k = 0;
	while (tokens[k])
	{
		if (tokens[k] == '>' || tokens[k] == '<')
		{
			k = handle_redirection(new_cmd, tokens, all, k, j);
			if (k == -1)
			{
				free(cmd);
				free(tokens);
				//free_cmd_node(new_cmd);;
				return (NULL);
			}
		}
		else
			k++;
	}
	free(cmd);
	free(tokens);
	//free(new_cmd);
	//free_cmd_node(new_cmd);
	return (new_cmd);
}

/*
** Fonction : parse_cmd
** Analyse une chaîne de commande et renvoie une liste de commandes.
*/

t_cmd	*parse_cmd(char *all)
{
	t_cmd	*cmd_lst = NULL;
	t_cmd	*new_cmd;
	char	*tmp;
	int		i;
	int		j;
	t_bool	in_quotes;

	tmp = ft_strdup(all);
	if (!tmp)
		return (NULL);
	i = -1;
	while (tmp[++i])
		tmp[i] = check_all_char(tmp[i]);
	i = 0;
	j = 0;
	in_quotes = FALSE;
	while (tmp[i])
	{
		in_quotes = handle_quotes(all[i], in_quotes);
		if (!in_quotes && (tmp[i] == '|' || !tmp[i]))
		{
			if (i > j)
			{
				printf("Enter in i > j  in parse_cmd\n");
				new_cmd = process_command_node(all, tmp, i, j);
				if (!new_cmd)
				{
					printf("error\n");
					free_cmd_list(cmd_lst);
					free(tmp);
					return (NULL);
				}
				ft_cmd_add_back(&cmd_lst, new_cmd);
			}
			if (tmp[i] == '|')
			{
				printf("Pipe detected\n");
				i++;
				while (all[i] && ft_isspace(all[i]))
					i++;
				if (all[i] == '\0')
				{
					printf("Error: Missing command after pipe.\n");
					free_cmd_list(cmd_lst);
					free(tmp);
					return (NULL);
				}
				j = i;
				continue;
			}
			i++;
			j = i;
			continue;
		}
		i++;
	}
	if (j < i)
	{
		printf("Enter in last cmd of parse_cmd\n");
		new_cmd = process_command_node(all, tmp, i, j);
		if (!new_cmd)
		{
			printf("error\n");
			free_cmd_list(cmd_lst);
			free(tmp);
			return (NULL);
		}
		ft_cmd_add_back(&cmd_lst, new_cmd);
	}
	free(tmp);
	return (cmd_lst);
}

/*
** count_tokens compte le nombre de tokens dans une chaîne 
** en fonction de types de caractères spécifiques.
*/
int count_tokens(const char *cmd_tokens)
{
	int count = 0;
	int i = 0;

	while (cmd_tokens[i])
	{
		while (cmd_tokens[i] == ' ')
			i++;
		if (cmd_tokens[i] == '\0')
			break;
		if (cmd_tokens[i] == '"' || cmd_tokens[i] == '\'')
		{
			char quote = cmd_tokens[i];
			count++;
			i++;
			while (cmd_tokens[i] && cmd_tokens[i] != quote)
				i++;
			if (cmd_tokens[i] == quote)
				i++;
		}
		else
		{
			count++;
			while (cmd_tokens[i] && cmd_tokens[i] != ' ' &&
				   cmd_tokens[i] != '"' && cmd_tokens[i] != '\'')
				i++;
		}
		while (cmd_tokens[i] == ' ')
			i++;
	}
	return count;
}


/*
** tokenise_command divise la chaîne de commande en tokens 
** selon les types 'c', '\'', '"', '|', '>', '<', et 'a'.
** reprend un peu la logique du schema parsing (voir notion)
*/

char **tokenise_command(char *cmd_str, char *cmd_tokens)
{
	int i;
	int start;
	int token_count;
	int total_tokens;
	char **tokens;
	char quote_char = '\0';
	int length;

	i = 0;
	start = 0;
	token_count = 0;
	total_tokens = count_tokens(cmd_tokens);
	tokens = (char **)malloc(sizeof(char *) * (total_tokens + 2));
	if (!tokens)
		return NULL;
	while (cmd_tokens[i])
	{
		while (cmd_tokens[i] == ' ')
			i++;
		if (cmd_tokens[i] == '\0')
			break;
		if (cmd_tokens[i] == '|' || cmd_tokens[i] == '>' || cmd_tokens[i] == '<')
		{
			length = 1;
			if ((cmd_tokens[i] == '>' && cmd_tokens[i + 1] == '>') ||
				(cmd_tokens[i] == '<' && cmd_tokens[i + 1] == '<'))
				length = 2; 
			tokens[token_count] = ft_strndup(&cmd_str[i], length);
			token_count++;
			i += length;
		}
		else if (cmd_tokens[i] == '"' || cmd_tokens[i] == '\'')
		{
			quote_char = cmd_tokens[i];
			i++;
			start = i;
			while (cmd_tokens[i] && cmd_tokens[i] != quote_char)
				i++;
			tokens[token_count] = ft_strndup(&cmd_str[start], i - start);
			token_count++;
			if (cmd_tokens[i] == quote_char)
				i++;
		}
		//else if (cmd_tokens[i] == '|')

		else
		{
			start = i;
			while (cmd_tokens[i] && cmd_tokens[i] != ' ' && cmd_tokens[i] != '|' &&
					cmd_tokens[i] != '>' && cmd_tokens[i] != '<' &&
					cmd_tokens[i] != '"' && cmd_tokens[i] != '\'')
				i++;
			tokens[token_count] = ft_strndup(&cmd_str[start],  i - start);
			token_count++;
		}
	}
	//free(cmd_str);
	//free(cmd_tokens);
	tokens[token_count] = NULL;
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
		return '|';
	else if (cmd == '>') // Redirection (a voir comment on fait pour >>)
		return '>';
	else if (cmd == '<') // Redirection (a voir comment on fait pour <<)
		return '<';
	else if (cmd == '"')
		return cmd; // Double quotes " "
	else if (cmd == '\'')
		return cmd; // Single quotes ' '
	else if (cmd == '$')
		return 'a'; // Arguments $ARG pas bien traité ... !!
	else if (!ft_isspace(cmd))
		return 'c';  // Command
	else 
		return ' '; // Pour les autres
}
