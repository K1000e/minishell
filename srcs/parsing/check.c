/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/16 06:19:59 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

t_bool	return_error(char *error, int exit_code)
{
	ft_fprintf(2, "%s\n", error);
	g_exit_code = exit_code;
	return (FALSE);
}

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
