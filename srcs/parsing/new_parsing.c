/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/19 17:23:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parse	*init_parse(char *line)
{
	t_parse	*parse;

	parse = malloc(sizeof(t_parse));
	if (!parse)
		return (NULL);
	parse->token_line = ft_strdup(line);
	parse->cmd_line = ft_strdup(line);
	if (!parse->token_line || !parse->cmd_line)
		return (free_parse(parse), NULL);
	check_char(parse);
	return (parse);
}

void	update_pipe_status(t_cmd *cmd_list, char *token_line, int *i)
{
	if (token_line[*i] == '|')
	{
		cmd_list->is_pipe = TRUE;
		(*i)++;
	}
	else
		cmd_list->is_pipe = FALSE;
}

t_cmd	*parse_command_loop(char *line, t_parse *parse, t_cmd *cmd_list)
{
	t_cmd	*new_cmd;
	int		i;
	int		j;

	new_cmd = NULL;
	i = 0;
	j = 0;
	while (TRUE)
	{
		if (!is_invalid_redir(parse, i) || (parse->token_line[i] == '|'
				&& !check_pipe_validity(line, i)))
			return (NULL);
		if (parse->token_line[i] == '\0' || (parse->token_line[i] == '|'
				&& check_pipe_validity(line, i)))
		{
			new_cmd = create_commands(parse, j, i, new_cmd);
			ft_cmd_add_back(&cmd_list, new_cmd);
			update_pipe_status(cmd_list, parse->token_line, &i);
			j = i;
		}
		if (!parse->token_line[i])
			break ;
		i++;
	}
	return (cmd_list);
}

t_cmd	*parse_command(char *line)
{
	t_parse	*parse;
	t_cmd	*cmd_list;

	cmd_list = NULL;
	parse = init_parse(line);
	if (!parse || !parse->token_line)
		return (free_parse(parse), NULL);
	cmd_list = parse_command_loop(line, parse, cmd_list);
	free_parse(parse);
	return (cmd_list);
}

char	**clear_redir(t_cmd *c)
{
	char	**new_args;
	int		i;
	int		x;

	c->nb_token -= (c->nb_infile * 2) - (c->nb_outfile * 2);
	new_args = ft_calloc(sizeof(char *), (c->nb_token + 1));
	i = -1;
	x = -1;
	while (c->args[++i])
	{
		if (!ft_strcmp(c->args_t[i], ">") || !ft_strcmp(c->args_t[i], ">>")
			|| !ft_strcmp(c->args_t[i], "<") || !ft_strcmp(c->args_t[i], "<<"))
			i++;
		else
			new_args[++x] = ft_strdup(c->args[i]);
	}
	i = -1;
	free_string_array(c->args);
	free_string_array(c->args_t);
	c->args = new_args;
	c->args_t = NULL;
	return (new_args);
}
