/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:57:23 by mabdessm          #+#    #+#             */
/*   Updated: 2024/12/17 00:36:24 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_infile(t_pipex *pipex, t_cmd *cmd, int j)
{
	int	fd;

	fd = open(cmd->in_file[j], O_RDONLY);
	if (fd < 0)
	{
		perror(cmd->in_file[j]);
		return (1);
	}
	if (pipex->file_i != -1)
		close(pipex->file_i);
	pipex->file_i = fd;
	return (0);
}

int	open_outfile(t_pipex *pipex, t_cmd *cmd, int k)
{
	int	fd;

	if (cmd->append[k] == 0)
		fd = open(cmd->out_file[k],
				O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else
		fd = open(cmd->out_file[k],
				O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (fd < 0)
	{
		perror(cmd->out_file[k]);
		return (1);
	}
	if (pipex->file_o != -1)
		close(pipex->file_o);
	pipex->file_o = fd;
	return (0);
}
