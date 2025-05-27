/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quote.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/17 00:37:11 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clear_quotes_append(t_parse *temp, t_parse *p, int *x, int i)
{
	temp->token_line[*x] = p->token_line[i];
	temp->cmd_line[*x] = p->cmd_line[i];
	(*x)++;
}

static void	clear_quotes_append_dbl(t_parse *temp, t_parse *p, int *x, int i)
{
	clear_quotes_append(temp, p, x, i);
	clear_quotes_append(temp, p, x, ++i);
}

static void	ft_copy_clean_cmd(t_parse *temp, t_parse *p)
{
	free(p->token_line);
	free(p->cmd_line);
	p->token_line = ft_strdup(temp->token_line);
	p->cmd_line = ft_strdup(temp->cmd_line);
	free(temp->token_line);
	free(temp->cmd_line);
	free(temp);
}

static void	clear_quotes_loop(t_parse *temp, t_parse *p, t_bool heredoc, int x)
{
	int	i;

	i = -1;
	while (p->token_line[++i])
	{
		if (p->token_line[i] == '<' && p->token_line[i + 1] == '<')
		{
			heredoc = TRUE;
			clear_quotes_append_dbl(temp, p, &x, i++);
			continue ;
		}
		if (heredoc && p->cmd_line[i] == '"' && p->cmd_line[i + 1] == '"')
		{
			clear_quotes_append_dbl(temp, p, &x, i++);
			heredoc = FALSE;
			continue ;
		}
		if (p->token_line[i] == 'e' && !heredoc)
			continue ;
		if (heredoc && p->token_line[i] != '<' && p->token_line[i] != 'e'
			&& p->token_line[i] != ' ')
			heredoc = FALSE;
		clear_quotes_append(temp, p, &x, i);
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
