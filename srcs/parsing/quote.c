/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/16 01:57:15 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

static void	clear_quotes_append(t_parse *temp, t_parse *parse, int *x, int i)
{
	temp->token_line[*x] = parse->token_line[i];
	temp->cmd_line[*x] = parse->cmd_line[i];
	(*x)++;
}

void	ft_copy_clean_cmd(t_parse *temp, t_parse *parse)
{
	free(parse->token_line);
	free(parse->cmd_line);
	parse->token_line = ft_strdup(temp->token_line);
	parse->cmd_line = ft_strdup(temp->cmd_line);
	free(temp->token_line);
	free(temp->cmd_line);
	free(temp);
}

void	clear_quotes_loop(t_parse *t, t_parse *p, t_bool heredoc, int x)
{
	int		i;

	i = -1;
	while (p->token_line[++i])
	{
		if (p->token_line[i] == '<' && p->token_line[i + 1] == '<')
		{
			heredoc = TRUE;
			clear_quotes_append(t, p, &x, i);
			clear_quotes_append(t, p, &x, ++i);
		}
		if (heredoc && p->cmd_line[i] == '"' && p->cmd_line[i + 1] == '"')
		{
			clear_quotes_append(t, p, &x, i);
			clear_quotes_append(t, p, &x, ++i);
			heredoc = FALSE;
		}
		if (!(p->token_line[i] == 'e' && !heredoc))
		{
			if (heredoc && p->token_line[i] != '<'
				&& p->token_line[i] != 'e' && p->token_line[i] != ' ')
				heredoc = FALSE;
			clear_quotes_append(t, p, &x, i);
		}
	}
}

void	clear_quotes(t_parse *parse)
{
	int		x;
	t_parse	*temp;
	t_bool	heredoc;

	temp = ft_calloc(sizeof(t_parse), 1);
	temp->token_line = ft_calloc(sizeof(char),
			ft_strlen(parse->token_line) + 1);
	temp->cmd_line = ft_calloc(sizeof(char), ft_strlen(parse->token_line) + 1);
	x = 0;
	heredoc = FALSE;
	clear_quotes_loop(temp, parse, heredoc, x);
	ft_copy_clean_cmd(temp, parse);
}
