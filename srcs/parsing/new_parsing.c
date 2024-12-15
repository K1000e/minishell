/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/15 07:01:05 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

t_cmd	*parse_command(char *line)
{
	t_cmd	*cmd_list;
	t_cmd	*new_cmd;
	t_parse	parse;
	int		i;
	int		j;

	cmd_list = NULL;
	new_cmd = NULL;
	parse.token_line = ft_strdup(line);
	parse.command_line = ft_strdup(line);
	if (!parse.token_line)
		return (NULL);
	check_char(&parse);
	i = 0;
	j = 0;
	while (TRUE)
	{
		if (parse.token_line[i] != '\0' && ((parse.token_line[i] == '>'
					|| parse.token_line[i] == '<')
				&& !check_redir(parse.token_line, parse.token_line[i], i)))
		{
			free(parse.token_line);
			free(parse.command_line);
			return (NULL);
		}
		if (parse.token_line[i] == '\0' || (parse.token_line[i] == '|'
				&& check_pipe_validity(line, i)))
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
			break ;
		i++;
	}
	free(parse.token_line);
	free(parse.command_line);
	return (cmd_list);
}

char	**clear_redir(t_cmd *cmd)
{
	char	**new_args;
	int		i;
	int		x;

	cmd->nb_token -= (cmd->nb_infile * 2) - (cmd->nb_outfile * 2);
	new_args = ft_calloc(sizeof(char *), (cmd->nb_token + 1));
	i = -1;
	x = -1;
	while (cmd->args[++i])
	{
		if (ft_strcmp(cmd->args_t[i], ">") == 0
			|| ft_strcmp(cmd->args_t[i], ">>") == 0
			|| ft_strcmp(cmd->args_t[i], "<") == 0
			|| ft_strcmp(cmd->args_t[i], "<<") == 0)
			i++;
		else
			new_args[++x] = ft_strdup(cmd->args[i]);
	}
	i = -1;
	while (cmd->args[++i])
	{
		free(cmd->args[i]);
		if (cmd->args_t[i])
			free(cmd->args_t[i]);
	}
	free(cmd->args);
	free(cmd->args_t);
	cmd->args = new_args;
	cmd->args_t = NULL;
	return (new_args);
}

int	count_tokens_(const char *cmd_tokens)
{
	int		count;
	int		i;
	char	token;

	count = 0;
	i = 0;
	while (cmd_tokens[i])
	{
		while (cmd_tokens[i] && cmd_tokens[i] == ' ')
			i++;
		token = cmd_tokens[i];
		while (cmd_tokens[i] && cmd_tokens[i] == token)
			i++;
		count++;
	}
	return (count);
}

void	clear_quotes(t_parse *parse)
{
	int		i;
	int		x;
	char	*res_token;
	char	*res_command;
	t_bool	heredoc;

	res_token = ft_calloc(sizeof(char), ft_strlen(parse->token_line) + 1);
	res_command = ft_calloc(sizeof(char), ft_strlen(parse->token_line) + 1);
	i = -1;
	x = 0;
	heredoc = FALSE;
	while (parse->token_line[++i])
	{
		if (parse->token_line[i] == '<' && parse->token_line[i + 1] == '<')
		{
			heredoc = TRUE;
			res_token[x] = parse->token_line[i];
			res_command[x++] = parse->command_line[i];
			res_token[x] = parse->token_line[++i];
			res_command[x++] = parse->command_line[i];
			continue ;
		}
		if (heredoc && parse->command_line[i] == '"'
			&& parse->command_line[i + 1] == '"')
		{
			res_token[x] = parse->token_line[i];
			res_command[x++] = parse->command_line[i++];
			res_token[x] = parse->token_line[i];
			res_command[x++] = parse->command_line[i];
			heredoc = FALSE;
			continue ;
		}
		if (parse->token_line[i] == 'e' && !heredoc)
			continue ;
		if (heredoc && parse->token_line[i] != '<'
			&& parse->token_line[i] != 'e' && parse->token_line[i] != ' ')
			heredoc = FALSE;
		res_token[x] = parse->token_line[i];
		res_command[x] = parse->command_line[i];
		x++;
	}
	free(parse->token_line);
	free(parse->command_line);
	parse->token_line = ft_strdup(res_token);
	parse->command_line = ft_strdup(res_command);
	free(res_token);
	free(res_command);
}

/* void	check_char(t_parse *parse)
{
	int	i;

	i = -1;
	while (parse->token_line[++i])
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
			while (parse->token_line[++i] && parse->token_line[i] != '"')
				parse->token_line[i] = 'c';
			parse->token_line[i] = 'e';
		}
		else if (parse->token_line[i] == '\'')
		{
			parse->token_line[i] = 'e';
			while (parse->token_line[++i] && parse->token_line[i] != '\'')
				parse->token_line[i] = 'c';
			parse->token_line[i] = 'e';
		}
		else if (!ft_isspace(parse->token_line[i]))
			parse->token_line[i] = 'c';
		else
			parse->token_line[i] = ' ';
	}
	clear_quotes(parse);
} */

/* char	**add_to_tab(char **tab, const char *str)
{
	int		i;
	char	**new_tab;

	i = 0;
	if (tab)
		while (tab[i] != NULL)
			i++;
	new_tab = malloc((i + 2) * sizeof(char *));
	new_tab[i] = strdup(str);
	if (!new_tab[i])
	{
		free(new_tab);
		return (NULL);
	}
	new_tab[i + 1] = NULL;
	while (--i >= 0)
		new_tab[i] = tab[i];
	if (tab)
		free(tab);
	return (new_tab);
}
 */