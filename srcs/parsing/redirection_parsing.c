/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parsing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/19 17:23:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_input_redirection(t_cmd *cmd, int i, int *in)
{
	if (cmd->args[i + 1])
	{
		cmd->redirection = TRUE;
		cmd->order_file = ft_join(cmd->order_file, "i");
		cmd->in_file[*in] = ft_strdup(cmd->args[i + 1]);
		return (i);
	}
	else
	{
		free_cmd_list(cmd);
		return (-1);
	}
}

static int	handle_output_redirection(t_cmd *cmd, int i, int *out)
{
	if (cmd->args[i + 1])
	{
		cmd->redirection = TRUE;
		cmd->order_file = ft_join(cmd->order_file, "o");
		cmd->out_file[*out] = ft_strdup(cmd->args[i + 1]);
		cmd->append[*out] = (ft_strcmp(cmd->args[i], ">>") == 0);
		return (i);
	}
	else
	{
		free_cmd_list(cmd);
		return (-1);
	}
}

static t_cmd	*handle_heredoc_redirection(t_cmd *new_cmd, int *i)
{
	new_cmd->redirection = TRUE;
	if (ft_strcmp(new_cmd->args_t[*i + 1], "ee") == 0)
		new_cmd->heredoc_delimiter = add_to_tab(new_cmd->heredoc_delimiter,
				"\0");
	else
		new_cmd->heredoc_delimiter = add_to_tab(new_cmd->heredoc_delimiter,
				new_cmd->args[*i + 1]);
	new_cmd->order_file = ft_join(new_cmd->order_file, "h");
	new_cmd->nb_heredoc++;
	new_cmd->heredoc_redirection = TRUE;
	(*i)++;
	return (new_cmd);
}

static t_cmd	*process_redirection(t_cmd *cmd, int *i, int *in, int *out)
{
	if (ft_strcmp(cmd->args[*i], ">") == 0
		|| ft_strcmp(cmd->args[*i], ">>") == 0)
	{
		*i = handle_output_redirection(cmd, *i, out);
		(*out)++;
		if (*i == -1)
			return (NULL);
		(*i)++;
	}
	else if (ft_strcmp(cmd->args[*i], "<<") == 0)
		return (handle_heredoc_redirection(cmd, i));
	else if (ft_strcmp(cmd->args[*i], "<") == 0)
	{
		*i = handle_input_redirection(cmd, *i, in);
		(*in)++;
		if (*i == -1)
			return (NULL);
		(*i)++;
	}
	return (cmd);
}

t_cmd	*handle_redirection(t_cmd *new_cmd)
{
	int	i;
	int	in;
	int	out;

	i = -1;
	in = 0;
	out = 0;
	if (new_cmd->nb_outfile)
	{
		new_cmd->out_file = ft_calloc(sizeof(char *),
				(new_cmd->nb_outfile + 1));
		new_cmd->append = ft_calloc(sizeof(int), (new_cmd->nb_outfile + 1));
	}
	if (new_cmd->nb_infile)
		new_cmd->in_file = ft_calloc(sizeof(char *), (new_cmd->nb_infile + 1));
	new_cmd->order_file = ft_strdup("");
	while (new_cmd->args[++i])
	{
		new_cmd = process_redirection(new_cmd, &i, &in, &out);
		if (!new_cmd)
			return (NULL);
	}
	new_cmd->args = clear_redir(new_cmd);
	return (new_cmd);
}
