/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/10/26 01:40:42 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

t_cmd *parse_command(char *line)
{
	printf("Parsing command\n");
	t_cmd	*cmd_list = NULL;
	t_cmd	*new_cmd = NULL;
	char	*token_line;
	int		i;
	int		j;

	token_line = ft_strdup(line);
	if (!token_line)
		return NULL;
	token_line = check_char(token_line);
	i = 0;
	j = 0;
	while (TRUE)
	{
		if (token_line[i] == '\0'
			|| (token_line[i] == '|' && check_pipe_validity(line, i)))
		{
			new_cmd = create_commands(line, token_line, j, i, new_cmd);
			ft_cmd_add_back(&cmd_list, new_cmd);
			if (token_line[i] == '|')
			{
				printf("Pipe detected\n");
				cmd_list->is_pipe = TRUE;
				i++;
			}
			j = i;
		}
		if (!token_line[i])
			break;
		i++;
	}
	return(cmd_list);
}

/* t_bool	handle_quotes(char current, t_bool in_quotes)
{
	if (current == '"' || current == '\'')
		return (!in_quotes);
	return (in_quotes);
} */

t_bool check_pipe_validity(char *line, int i)
{
	if (line[i] == '|')
	{
		printf("Pipe detected\n");
		i++;
		while (line[i] && ft_isspace(line[i]))
			i++;
		if (line[i] == '\0' || line[i] == '|')
		{
			printf("Error: Missing command after pipe.\n");
			/* free_cmd_list(cmd_lst);
			free(tmp);
			return (NULL); */
			return FALSE;
		}
	}
	return TRUE;
}

t_cmd *create_commands(char *line, char *token_line, int start, int end, t_cmd *cmd)
{
	char	*command;
	char	*tokens;
	
	printf("Creating command\n");
	command = ft_strndup(&line[start], end - start);
	tokens = ft_strndup(&token_line[start], end - start);
	cmd = create_cmd_node_(command, tokens, cmd);
	return (cmd);
}

int count_redirection(char *cmd, char type)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (cmd[i])
	{
		if (cmd[i] == type)
		{
			i++;
			if (cmd[i] == type)
				i++;
			count++;
		}
		i++;
	}
	return (count);
}

t_cmd *create_cmd_node_(char *cmd_str, char *cmd_tokens, t_cmd *cmd)
{
	cmd = malloc(sizeof(t_cmd));
	printf("Creating command node\n");
	cmd->cmd = ft_strdup(cmd_str);
	cmd->next = NULL;
	cmd->append = 0;
	cmd->redirection = FALSE;
	cmd->nb_infile = count_redirection(cmd_tokens, '<');
	cmd->nb_outfile = count_redirection(cmd_tokens, '>');
	if (cmd->nb_outfile)
		cmd->out_file = ft_calloc(sizeof(char), (cmd->nb_infile + 1));
	else
		cmd->out_file = NULL;
	if (cmd->nb_infile)
		cmd->in_file = ft_calloc(sizeof(char), (cmd->nb_outfile + 1));
	else
		cmd->in_file = NULL;
	cmd->args = make_argument(cmd_str, cmd_tokens);
	cmd = handle_redirection_(cmd);
	return (cmd);
}

char **make_argument(char *cmd_str, char *cmd_tokens)
{
	char **args;
	int nb_args;
	
	printf("Making arguments\n");
	nb_args = count_tokens_(cmd_tokens);
	args = malloc(sizeof(char *) * (nb_args + 1));
	args = parse_args(cmd_str, cmd_tokens, args);
	return (args);
}

int count_tokens_(const char *cmd_tokens)
{
	int count = 0;
	//t_bool	in_quotes;
	//char quote;
	int i = 0;
	char token;
	printf("Counting tokens\n");
	printf("cmd_tokens = %s\n", cmd_tokens);
	while (cmd_tokens[i])
	{
		while (cmd_tokens[i] && cmd_tokens[i] == ' ')
			i++;
		token = cmd_tokens[i];
		printf("token = %c\n", token);
		while(cmd_tokens[i] && cmd_tokens[i] == token)
			i++;
		count++;
	}
	return (count);
}

char **parse_args(char *cmd_str, char *cmd_tokens, char **args)
{
	int start;
	int i;
	//char quote_char;
	int nb_args;
	char token;

	printf("Parsing arguments\n");
	i = 0;
	start = 0;
	nb_args = 0;
	while (cmd_tokens[i])
	{
		while (cmd_tokens[i] && cmd_tokens[i] == ' ')
			i++;
		//while (cmd_tokens[i] && cmd_tokens[i])
		token = cmd_tokens[i];
		start = i;
		while (cmd_tokens[i] && cmd_tokens[i] == token)
			i++;
		args[nb_args] = ft_strndup(&cmd_str[start],  i - start);
			nb_args++;
		/* 
		if (cmd_tokens[i] == '"')
		{
			start = i;
			while (cmd_tokens[i] == '"')
				i++;
			args[nb_args] = ft_strndup(&cmd_str[start], i - start);
			nb_args++;
		}
		if (cmd_tokens[i] == '\'')
		{
			start = i;
			while (cmd_tokens[i] == '\'')
				i++;
			args[nb_args] = ft_strndup(&cmd_str[start], i - start);
			nb_args++;
		}
		else
		{
			start = i;
			while (cmd_tokens[i] && cmd_tokens[i] != ' '
				&& cmd_tokens[i] != '>' && cmd_tokens[i] != '<'
				&& cmd_tokens[i] != '"' && cmd_tokens[i] != '\'')
				i++;
			args[nb_args] = ft_strndup(&cmd_str[start],  i - start);
			nb_args++;
		} */
	}
	args[nb_args] = NULL;
	return (args);
}

char* check_char(char *cmd)
{
	int i;

	printf("Checking characters\n");
	i = -1;
	while(cmd[++i])
	{
		if (cmd[i] == '|') // Pipe
			cmd[i] = '|';
		else if (cmd[i] == '>') // Redirection (a voir comment on fait pour >>)
			cmd[i] = '>';
		else if (cmd[i] == '<') // Redirection (a voir comment on fait pour <<)
			cmd[i] = '<';
		else if (cmd[i] == '"')
		{
			while(cmd[i] && cmd[++i] != '"')
				cmd[i] = '"';
			i++;
		}
		else if (cmd[i] == '\'')
		{
			while(cmd[i] && cmd[++i] != '\'')
				cmd[i] = '\'';
			i++;
		}
		else if (cmd[i] == '$')
			cmd[i] = 'a'; // Arguments $ARG pas bien traité ... !!
		else if (!ft_isspace(cmd[i]))
			cmd[i] = 'c';  // Command
		else 
			cmd[i] = ' '; // Pour les espaces
	}
	return (cmd);
}

int	handle_output_redirection_(t_cmd *cmd, char **args, int i, int *out)
{
	printf("here\n");
	if (args[i + 1])
	{
		cmd->redirection = TRUE;
		printf("out -> %d \n", *out);
		printf("args[%d] -> %s\n", i, args[i + 1]);
		cmd->out_file[*out] = ft_strdup(args[i + 1]);
		printf("out_file -> %s\n", cmd->out_file[*out]);
		cmd->out_file[*out + 1] = NULL;
		cmd->append[*out] = (ft_strcmp(args[i], ">>") == 0);
		printf("append -> %d\n", cmd->append[*out]);
		cmd->nb_infile = *out + 1;
		printf("nb_infile -> %d\n", cmd->nb_infile);
		//out++;
		return (i + 2);
	}
	else
	{
		printf("Error: Missing file for redirection.\n");
		free_cmd_list(cmd);
		return (-1);
	}
}

t_cmd	*handle_redirection_(t_cmd *new_cmd)
{
	//char	*file;
	int		i;
	int		in;
	int		out;

	printf("Handling redirection\n");
	i = 0;
	in = 0;
	out = 0;
	
	while (new_cmd->args[i])
	{
		if (ft_strcmp(new_cmd->args[i], ">") == 0
			|| ft_strcmp(new_cmd->args[i], ">>") == 0)
		{
			printf("ouput\n");
			i = handle_output_redirection_(new_cmd, new_cmd->args, i, &out);
			out++;
			if (i == -1)
				return NULL;
		}
		else if (ft_strcmp(new_cmd->args[i], "<") == 0)
		{
			printf("input\n");
			i = handle_input_redirection(new_cmd, new_cmd->args, i, &in);
			//in++;
			if (i == -1)
				return NULL;
		}
		i++;
	}
	return (new_cmd);
}

