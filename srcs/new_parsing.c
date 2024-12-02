/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/01 23:43:36 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

t_bool check_pipe(char *token)
{
	int i;
	t_bool char_found;

	i = 0;
	while (token[i])
	{
		if (token[i] != '|' && token[i] != ' ')
			char_found = TRUE;
		if (token[i] == '|')
		{
			if (token[i + 1] == '|' || token[i + 1] == '\0'
				|| i == 0 || !char_found)
			{
				ft_fprintf(2, "bash: syntax error near unexpected token `|'\n");
				g_exit_code = 2;
				return FALSE;
			}
			char_found = FALSE;
		}
		i++;
	}
	return TRUE;
}

t_cmd *parse_command(char *line)
{
	t_cmd	*cmd_list = NULL;
	t_cmd	*new_cmd = NULL;
	t_parse	parse;
	int		i;
	int		j;

	parse.token_line = ft_strdup(line);
	parse.command_line = ft_strdup(line);
	if (!parse.token_line)
		return NULL;
	check_char(&parse);
	if (!check_pipe(parse.token_line))
		return NULL;
	i = 0;
	j = 0;
	while (TRUE)
	{
		if (parse.token_line[i] == '\0'
			|| (parse.token_line[i] == '|' && check_pipe_validity(line, i)))
		{
			new_cmd = create_commands(&parse, j, i, new_cmd);
			ft_cmd_add_back(&cmd_list, new_cmd);
			if (parse.token_line[i] == '|')
			{
				cmd_list->is_pipe = TRUE;
				i++;
			}
			else
				cmd_list->is_pipe = FALSE;
			j = i;
		}
		if (!parse.token_line[i])
			break;
		i++;
	}
	free(parse.token_line);
	free(parse.command_line);
	return(cmd_list);
}

t_bool check_pipe_validity(char *line, int i)
{
	int y;

	y = 0;
	if (i == 0)
	{
		ft_fprintf(2, "syntax error near unexpected token `|'\n");
		return FALSE;
	}
	while (y < i)
	{
		if (line[i] == '\0')
		{
			ft_fprintf(2, "syntax error near unexpected token `|'\n");
			return FALSE;
		}
		y++;
	} 
	if (line[i] == '|')
	{
		i++;
		while (line[i] && ft_isspace(line[i]))
			i++;
		if (line[i] == '\0' || line[i] == '|')
		{
			ft_fprintf(2, "syntax error near unexpected token `|'\n");
			return FALSE;
		}
	}
	return TRUE;
}

t_cmd *create_commands(t_parse *parse, int start, int end, t_cmd *cmd)
{
	char	*command;
	char	*tokens;

	command = ft_strndup(&parse->command_line[start], end - start);
	tokens = ft_strndup(&parse->token_line[start], end - start);
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

char	**clear_redir(t_cmd *cmd)
{
	char **new_args;
	int i;
	int x;

	cmd->nb_token -= (cmd->nb_infile * 2) - (cmd->nb_outfile * 2);
	new_args = ft_calloc(sizeof(char *), (cmd->nb_token + 1));
	i = -1;
	x = 0;
	while (cmd->args[++i])
	{
		if (ft_strcmp(cmd->args_token[i], ">") == 0
			|| ft_strcmp(cmd->args_token[i], ">>") == 0
			|| ft_strcmp(cmd->args_token[i], "<") == 0
			|| ft_strcmp(cmd->args_token[i], "<<") == 0)
			i++;
		else
		{
			new_args[x] = ft_strdup(cmd->args[i]);
			x++;
		}
	}
	i = -1;
	while (cmd->args[++i])
	{
		free(cmd->args[i]);
		if (cmd->args_token[i])
			free(cmd->args_token[i]);	
	}
	free(cmd->args);
	free(cmd->args_token);
	return(new_args);
}

t_cmd *create_cmd_node_(char *cmd_str, char *cmd_tokens, t_cmd *cmd)
{
	cmd = malloc(sizeof(t_cmd));
	cmd->cmd = ft_strdup(cmd_str);
	cmd->token = ft_strdup(cmd_tokens);
	cmd->next = NULL;
	cmd->append = 0;
	cmd->redirection = FALSE;
	cmd->nb_infile = count_redirection(cmd_tokens, '<');
	cmd->nb_outfile = count_redirection(cmd_tokens, '>');
	if (cmd->nb_outfile)
	{
		cmd->out_file = ft_calloc(sizeof(char *), (cmd->nb_outfile + 1));
		cmd->append = ft_calloc(sizeof(int), (cmd->nb_outfile + 1));
	}
	else
		cmd->out_file = NULL;
	if (cmd->nb_infile)
		cmd->in_file = ft_calloc(sizeof(char *), (cmd->nb_infile + 1));
	else
		cmd->in_file = NULL;
	make_argument(cmd_str, cmd_tokens, cmd);
	if (cmd->nb_infile || cmd->nb_outfile)
	{
		cmd = handle_redirection_(cmd);
		cmd->args = clear_redir(cmd);
	}
	free(cmd_str);
	free(cmd_tokens);
	return (cmd);
}

void make_argument(char *cmd_str, char *cmd_tokens, t_cmd *cmd)
{
	cmd->nb_token = count_tokens_(cmd_tokens);
	cmd->args = malloc(sizeof(char *) * (cmd->nb_token + 1));
	cmd->args_token = malloc(sizeof(char *) * (cmd->nb_token + 1));
	parse_args(cmd_str, cmd_tokens, cmd);
}

int count_tokens_(const char *cmd_tokens)
{
	int count = 0;
	int i = 0;
	char token;

	while (cmd_tokens[i])
	{
		while (cmd_tokens[i] && cmd_tokens[i] == ' ')
			i++;
		token = cmd_tokens[i];
		while(cmd_tokens[i] && cmd_tokens[i] == token)
			i++;
		count++;
	}
	return (count);
}

void parse_args(char *cmd_str, char *cmd_tokens, t_cmd *cmd )
{
	int start;
	int i;
	int nb_args;
	char token;

	i = 0;
	start = 0;
	nb_args = 0;
	while (cmd_tokens[i])
	{
		while (cmd_tokens[i] && cmd_tokens[i] == ' ')
			i++;
		if (cmd_tokens[i])
		{
			token = cmd_tokens[i];
			start = i;
			while (cmd_tokens[i] && cmd_tokens[i] == token)
				i++;
			cmd->args[nb_args] = ft_strndup(&cmd_str[start], i - start);
			cmd->args_token[nb_args] = ft_strndup(&cmd_tokens[start], i - start);
			nb_args++;
		}
	}
	cmd->args[nb_args] = NULL;
	cmd->args_token[nb_args] = NULL;
}

void	clear_quotes(t_parse *parse)
{
	int		i;
	int		x;
	char	*res_token;
	char	*res_command;

	res_token = ft_calloc(sizeof(char), ft_strlen(parse->token_line) + 1);
	res_command = ft_calloc(sizeof(char), ft_strlen(parse->token_line) + 1);
	i = -1;
	x = 0;
	while (parse->token_line[++i])
	{
		if (parse->token_line[i] == 'e')
			continue ;
		else
		{
			res_token[x] = parse->token_line[i];
			res_command[x] = parse->command_line[i];
			x++;
		}
	}
	free(parse->token_line);
	free(parse->command_line);
	parse->token_line = ft_strdup(res_token);
	parse->command_line = ft_strdup(res_command);
	free(res_token);
	free(res_command);
}

void check_char(t_parse *parse)
{
	int i;

	i = -1;
	while(parse->token_line[++i])
	{
		if (parse->token_line[i] == '|')
			parse->token_line[i] = '|';
		else if (parse->token_line[i] == '>')
			parse->token_line[i] = '>';
		else if (parse->token_line[i] == '<')
			parse->token_line[i] = '<';
 		else if (parse->token_line[i] == '"')
		{
			parse->token_line[i] = 'e';
			while(parse->token_line[++i] && parse->token_line[i] != '"')
				parse->token_line[i] = 'c';
			parse->token_line[i] = 'e';
		}
		else if (parse->token_line[i] == '\'')
		{
			parse->token_line[i] = 'e';
			while(parse->token_line[++i] && parse->token_line[i] != '\'')
				parse->token_line[i] = 'c';
			parse->token_line[i] = 'e';
		}
		else if (!ft_isspace(parse->token_line[i]))
			parse->token_line[i] = 'c';
		else 
			parse->token_line[i] = ' ';
	}
	clear_quotes(parse);
}

int	handle_input_redirection(t_cmd *cmd, int i, int *in)
{
	if (cmd->args[i + 1])
	{
		cmd->redirection = TRUE;
		cmd->order_file = ft_join(cmd->order_file, "i");
		cmd->in_file[*in] = ft_strdup(cmd->args[i + 1]);
		return (i);
	}
	else
	{
		free_cmd_list(cmd);
		return (-1);
	}
}

int	handle_output_redirection_(t_cmd *cmd, int i, int *out)
{
	if (cmd->args[i + 1])
	{
		cmd->redirection = TRUE;
		cmd->order_file = ft_join(cmd->order_file, "o");
		cmd->out_file[*out] = ft_strdup(cmd->args[i + 1]);
		cmd->append[*out] = (ft_strcmp(cmd->args[i], ">>") == 0);
		return (i);
	}
	else
	{
		free_cmd_list(cmd);
		return (-1);
	}
}

t_cmd	*handle_redirection_(t_cmd *new_cmd)
{
	int		i;
	int		in;
	int		out;

	i = -1;
	in = 0;
	out = 0;
	//order = 0;
	new_cmd->order_file = ft_strdup("");
	while (new_cmd->args[++i])
	{
		if (ft_strcmp(new_cmd->args[i], ">") == 0
			|| ft_strcmp(new_cmd->args[i], ">>") == 0)
		{
			i = handle_output_redirection_(new_cmd, i, &out);
			out++;
			if (i == -1)
				return NULL;
			i++;
		}
		else if (ft_strcmp(new_cmd->args[i], "<") == 0)
		{
			i = handle_input_redirection(new_cmd, i, &in);
			in++;
			if (i == -1)
				return NULL;
			i++;
		}
	}
	return (new_cmd);
}

