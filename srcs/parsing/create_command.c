/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/15 06:58:44 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

t_cmd	*create_commands(t_parse *parse, int start, int end, t_cmd *cmd)
{
	char	*command;
	char	*tokens;

	command = ft_strndup(&parse->command_line[start], end - start);
	tokens = ft_strndup(&parse->token_line[start], end - start);
	cmd = create_cmd_node_(command, tokens, cmd);
	free(command);
	free(tokens);
	return (cmd);
}

t_cmd	*create_cmd_node_(char *cmd_str, char *cmd_tokens, t_cmd *cmd)
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
	cmd->nb_infile = count_redirection(cmd_tokens, '<');
	cmd->nb_outfile = count_redirection(cmd_tokens, '>');
	cmd->nb_heredoc = 0;
	cmd->heredoc_delimiter = NULL;
	cmd->out_file = NULL;
	cmd->in_file = NULL;
	cmd->order_file = NULL;
	make_argument(cmd_str, cmd_tokens, cmd);
	return (cmd);
}
