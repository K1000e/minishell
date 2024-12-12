/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 23:26:21 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/03 00:11:07 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

t_bool	count_redir(const char *line)
{
	int	j;

	j = 0;
	while (line[j])
	{
		if (line[j] == '>')
		{
			if (line[j + 1] && line[j + 1] == '>')
			{
				if (line[j + 2] && line[j + 2] == '>')
					return (FALSE);
				j += 2;
			}
			else
				j++;
		}
		else if (line[j] == '<')
		{
			if (line[j + 1] && line[j + 1] == '<')
			{
				if (line[j + 2] && line[j + 2] == '<')
					return (FALSE);
				j += 2;
			}
			else
				j++;
		}
		else
			j++;
	}
	return (TRUE);
}

t_bool	match_quotes(char *line)
{
	int		i;
	t_bool	d_q;
	t_bool	s_q;

	i = -1;
	d_q = FALSE;
	s_q = FALSE;
	while (line[++i])
	{
		if (line[i] == '"' && d_q == FALSE && s_q == FALSE)
			d_q = TRUE;
		else if (line[i] == '"' && d_q == TRUE && s_q == FALSE)
			d_q = FALSE;
		else if (line[i] == '\'' && d_q == FALSE && s_q == FALSE)
			s_q = TRUE;
		else if (line[i] == '\'' && d_q == FALSE && s_q == TRUE)
			s_q = FALSE;
	}
	if (d_q == TRUE || s_q == TRUE)
	{
		printf("Error: Unmatched quote\n");
		return (FALSE);
	}
	return (TRUE);
}

t_bool	is_valid_command_format(const char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i] && !isspace(cmd[i]) && cmd[i] != '"' && cmd[i] != '\''
		&& cmd[i] != '|' && cmd[i] != '>' && cmd[i] != '<')
		i++;
	if (cmd[i] && cmd[i] != ' ' && cmd[i] != '|' && cmd[i] != '>'
		&& cmd[i] != '<' && cmd[i] != '\0')
		return (FALSE);
	return (TRUE);
}
