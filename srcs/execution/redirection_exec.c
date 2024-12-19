/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_exec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:57:23 by mabdessm          #+#    #+#             */
/*   Updated: 2024/12/19 17:23:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redirection_exit(t_bool should_exit)
{
	if (should_exit)
		exit(1);
	return (1);
}

static void	handle_file_descriptors(t_pipex *pipex, t_cmd *cmd)
{
	if (pipex->file_i != -1)
		dup2(pipex->file_i, STDIN_FILENO);
	if (pipex->file_i != -1 && cmd->heredoc_redirection)
		reopen_heredoc(pipex, FALSE);
	else if (pipex->file_i != -1)
		close(pipex->pipe_fd[1]);
	if (pipex->file_o != -1)
	{
		dup2(pipex->file_o, STDOUT_FILENO);
		close(pipex->file_o);
	}
}

static int	handle_file_redirection(t_pipex *pipex, t_cmd *cmd, t_file *info,
	t_bool should_exit)
{
	if (info->type == 'h')
	{
		if (info->h < cmd->nb_heredoc - 1)
			handle_heredoc(cmd->heredoc_delimiter[info->h], pipex, FALSE);
		else
			handle_heredoc(cmd->heredoc_delimiter[info->h], pipex, TRUE);
		(info->h)++;
	}
	else if (info->type == 'i' && open_infile(pipex, cmd, info->j) == 1)
		return (redirection_exit(should_exit));
	else if (info->type == 'o' && open_outfile(pipex, cmd, info->k) == 1)
		return (redirection_exit(should_exit));
	else if (info->type == 'i')
		(info->j)++;
	else if (info->type == 'o')
		(info->k)++;
	else
		return (redirection_exit(should_exit));
	return (0);
}

int	redirection_exec(t_cmd *cmd, t_pipex *pipex, t_bool should_exit)
{
	int		i;
	t_file	*info;

	info = malloc(sizeof(t_file));
	if (cmd->order_file == NULL)
		return (1);
	info->j = 0;
	info->k = 0;
	info->h = 0;
	i = -1;
	while (cmd->order_file[++i])
	{
		info->type = cmd->order_file[i];
		if (handle_file_redirection(pipex, cmd, info, should_exit))
		{
			free(info);
			return (1);
		}
	}
	handle_file_descriptors(pipex, cmd);
	free(info);
	return (0);
}
