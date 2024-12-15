/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/15 07:24:51 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

t_bool	check_pipe(char *token)
{
	int		i;
	t_bool	char_found;

	i = 0;
	while (token[i])
	{
		if (token[i] != '|' && token[i] != ' ')
			char_found = TRUE;
		if (token[i] == '|')
		{
			if (token[i + 1] == '|' || token[i + 1] == '\0' || i == 0
				|| !char_found)
			{
				ft_fprintf(2, "bash: syntax error near unexpected token `|'\n");
				g_exit_code = 2;
				return (FALSE);
			}
			char_found = FALSE;
		}
		i++;
	}
	return (TRUE);
}

t_bool	check_pipe_validity(char *line, int i)
{
	int	y;

	y = 0;
	if (i == 0)
	{
		ft_fprintf(2, "syntax error near unexpected token `|'\n");
		return (FALSE);
	}
	while (y < i)
	{
		if (line[i] == '\0')
		{
			ft_fprintf(2, "syntax error near unexpected token `|'\n");
			return (FALSE);
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
			return (FALSE);
		}
	}
	return (TRUE);
}
