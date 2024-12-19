/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/19 17:23:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_double_quotes(t_parse *parse, int *i)
{
	parse->token_line[*i] = 'e';
	while (parse->token_line[++(*i)] && parse->token_line[*i] != '"')
		parse->token_line[*i] = 'c';
	parse->token_line[*i] = 'e';
}

static void	handle_single_quotes(t_parse *parse, int *i)
{
	parse->token_line[*i] = 'e';
	while (parse->token_line[++(*i)] && parse->token_line[*i] != '\'')
		parse->token_line[*i] = 'c';
	parse->token_line[*i] = 'e';
}

static void	handle_special_characters(t_parse *parse, int *i)
{
	if (parse->token_line[*i] == '|')
		parse->token_line[*i] = '|';
	else if (parse->token_line[*i] == '>')
		parse->token_line[*i] = '>';
	else if (parse->token_line[*i] == '<')
		parse->token_line[*i] = '<';
	else if (parse->token_line[*i] == '"')
		handle_double_quotes(parse, i);
	else if (parse->token_line[*i] == '\'')
		handle_single_quotes(parse, i);
	else if (!ft_isspace(parse->token_line[*i]))
		parse->token_line[*i] = 'c';
	else
		parse->token_line[*i] = ' ';
}

void	tokenize_char(t_parse *parse)
{
	int	i;

	i = -1;
	while (parse->token_line[++i])
		handle_special_characters(parse, &i);
	clear_quotes(parse);
}
