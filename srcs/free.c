/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/17 00:37:11 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(t_pipex *pipex)
{
	if (pipex->pipe_fd[0] != -1)
		close(pipex->pipe_fd[0]);
	if (pipex->pipe_fd[1] != -1)
		close(pipex->pipe_fd[1]);
	if (pipex->file_i != -1)
		close(pipex->file_i);
	if (pipex->file_o != -1)
		close(pipex->file_o);
	if (pipex)
		free(pipex);
}

void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	free_parse(t_parse *parse)
{
	if (!parse)
		return ;
	if (parse->token_line)
		free(parse->token_line);
	if (parse->cmd_line)
		free(parse->cmd_line);
	free(parse);
}

static void	free_cmd_node(t_cmd *cmd_node)
{
	if (cmd_node->cmd)
		free(cmd_node->cmd);
	if (cmd_node->token)
		free(cmd_node->token);
	if (cmd_node->args)
		free_string_array(cmd_node->args);
	if (cmd_node->args_t)
		free_string_array(cmd_node->args_t);
	if (cmd_node->out_file)
		free_string_array(cmd_node->out_file);
	if (cmd_node->in_file)
		free_string_array(cmd_node->in_file);
	if (cmd_node->append)
		free(cmd_node->append);
	if (cmd_node->order_file)
		free(cmd_node->order_file);
	if (cmd_node->heredoc_delimiter)
		free_string_array(cmd_node->heredoc_delimiter);
	free(cmd_node);
}

void	free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmd_list;
	while (current)
	{
		next = current->next;
		free_cmd_node(current);
		current = next;
	}
}
