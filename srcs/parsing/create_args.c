/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/19 17:23:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_tokens(const char *cmd_tokens)
{
	int		count;
	int		i;
	char	token;

	count = 0;
	i = 0;
	while (cmd_tokens[i])
	{
		while (cmd_tokens[i] && cmd_tokens[i] == ' ')
			i++;
		token = cmd_tokens[i];
		while (cmd_tokens[i] && cmd_tokens[i] == token)
			i++;
		count++;
	}
	return (count);
}

static void	parse_args(char *cmd_str, char *cmd_tokens, t_cmd *cmd)
{
	int		start;
	int		i;
	int		nb_args;
	char	token;

	i = 0;
	start = 0;
	nb_args = 0;
	while (cmd_tokens[i])
	{
		while (cmd_tokens[i] && cmd_tokens[i] == ' ')
			i++;
		if (cmd_tokens[i])
		{
			token = cmd_tokens[i];
			start = i;
			while (cmd_tokens[i] && cmd_tokens[i] == token)
				i++;
			cmd->args[nb_args] = ft_strndup(&cmd_str[start], i - start);
			cmd->args_t[nb_args] = ft_strndup(&cmd_tokens[start], i - start);
			nb_args++;
		}
	}
	cmd->args[nb_args] = NULL;
	cmd->args_t[nb_args] = NULL;
}

void	make_argument(char *cmd_str, char *cmd_tokens, t_cmd *cmd)
{
	cmd->nb_token = count_tokens(cmd_tokens);
	cmd->args = malloc(sizeof(char *) * (cmd->nb_token + 1));
	cmd->args_t = malloc(sizeof(char *) * (cmd->nb_token + 1));
	if (!cmd->args || !cmd->args_t)
	{
		if (cmd->args)
			free(cmd->args);
		if (cmd->args_t)
			free(cmd->args_t);
		return ;
	}
	parse_args(cmd_str, cmd_tokens, cmd);
	if (cmd->nb_infile || cmd->nb_outfile)
		cmd = handle_redirection(cmd);
}
