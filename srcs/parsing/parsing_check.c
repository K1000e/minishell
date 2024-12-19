/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/19 17:23:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	check_pipe_validity(char *line, int i)
{
	int	y;

	y = 0;
	if (i == 0)
		return (return_error(ERR_SYNTAX"`|'", 2));
	while (y < i)
	{
		if (line[i] == '\0')
			return (return_error(ERR_SYNTAX"`|'", 2));
		y++;
	}
	if (line[i] == '|')
	{
		i++;
		while (line[i] && ft_isspace(line[i]))
			i++;
		if (line[i] == '\0' || line[i] == '|')
			return (return_error(ERR_SYNTAX"`|'", 2));
	}
	return (TRUE);
}

t_bool	is_invalid_redir(t_parse *parse, int i)
{
	if (parse->token_line[i] != '\0' && ((parse->token_line[i] == '>'
				|| parse->token_line[i] == '<')
			&& !check_redir(parse->token_line, parse->token_line[i], i)))
		return (FALSE);
	return (TRUE);
}
