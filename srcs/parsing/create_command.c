/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/17 01:02:48 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_redir_parsing(char *cmd, char type)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!cmd)
		return (0);
	while (cmd[i])
	{
		if (cmd[i] == type)
		{
			i++;
			if (cmd[i] == type)
				i++;
			count++;
		}
		i++;
	}
	return (count);
}

static t_cmd	*create_cmd_node(char *cmd_str, char *cmd_tokens, t_cmd *cmd)
{
	cmd = malloc(sizeof(t_cmd));
	cmd->cmd = ft_strdup(cmd_str);
	cmd->token = ft_strdup(cmd_tokens);
	if (!cmd->cmd || !cmd->token)
	{
		free(cmd->cmd);
		free(cmd->token);
		free(cmd);
		free(cmd_str);
		free(cmd_tokens);
		return (NULL);
	}
	cmd->next = NULL;
	cmd->append = 0;
	cmd->redirection = FALSE;
	cmd->heredoc_redirection = FALSE;
	cmd->nb_infile = count_redir_parsing(cmd_tokens, '<');
	cmd->nb_outfile = count_redir_parsing(cmd_tokens, '>');
	cmd->nb_heredoc = 0;
	cmd->heredoc_delimiter = NULL;
	cmd->out_file = NULL;
	cmd->in_file = NULL;
	cmd->order_file = NULL;
	make_argument(cmd_str, cmd_tokens, cmd);
	return (cmd);
}

t_cmd	*create_commands(t_parse *parse, int start, int end, t_cmd *cmd)
{
	char	*command;
	char	*tokens;

	command = ft_strndup(&parse->cmd_line[start], end - start);
	tokens = ft_strndup(&parse->token_line[start], end - start);
	cmd = create_cmd_node(command, tokens, cmd);
	free(command);
	free(tokens);
	return (cmd);
}
