/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 23:31:25 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/16 23:21:32 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

t_bool	check_redir(const char *line, char token, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		if (line[i] == '\0')
			return (return_error(ERR_SYNTAX"`newline'", 2));
		j++;
	}
	if (line[i] == token)
	{
		i++;
		if (line[i] == token)
			i++;
		while (line[i] && ft_isspace(line[i]))
			i++;
		if (line[i] == '\0' || line[i] == token)
			return (return_error(ERR_SYNTAX"`newline'", 2));
		if (line[i] == '|')
			return (return_error(ERR_SYNTAX"`|'", 2));
	}
	return (TRUE);
}

static t_bool	count_redirection_err(const char *line, char token, int *i)
{
	if (line[*i + 1] && line[*i + 1] == token)
	{
		if (line[*i + 2] && line[*i + 2] == token)
		{
			if (token == '>')
				return (return_error(ERR_SYNTAX"`>'", 2));
			else
				return (return_error(ERR_SYNTAX"`<'", 2));
		}
		*i += 2;
	}
	else
		(*i)++;
	return (TRUE);
}

t_bool	count_redir(const char *line)
{
	int	j;

	j = 0;
	while (line[j])
	{
		if (line[j] == '>')
		{
			if (!count_redirection_err(line, '>', &j))
				return (FALSE);
		}
		else if (line[j] == '<')
		{
			if (!count_redirection_err(line, '<', &j))
				return (FALSE);
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
		ft_fprintf(2, "bash: unmatched quote\n");
		return (FALSE);
	}
	return (TRUE);
}
