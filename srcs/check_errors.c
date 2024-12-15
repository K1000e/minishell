/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 23:31:25 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/15 04:06:56 by cgorin           ###   ########.fr       */
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
		{
			ft_fprintf(2, "syntax error near unexpected token `newline'\n");
			return (FALSE);
		}
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
		{
			printf("line[i] = %c\n", line[i]);
			ft_fprintf(2, "syntax error near unexpected token `newline'\n");
			return (FALSE);
		}
		if (line[i] == '|')
		{
			ft_fprintf(2, "syntax error near unexpected token `|'\n", line[i]);
			return (FALSE);
		}
	}
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
			if (line[j + 1] && line[j + 1] == '>')
			{
				if (line[j + 2] && line[j + 2] == '>')
				{
					ft_fprintf(2, "syntax error near unexpected token `>'\n");
					return (FALSE);
				}
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
				{
					ft_fprintf(2,
						"bash: syntax error near unexpected token `<'\n");
					return (FALSE);
				}
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
		ft_fprintf(2, "bash: unmatched quote\n");
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
