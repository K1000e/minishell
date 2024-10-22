/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_open_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 13:29:18 by cgorin            #+#    #+#             */
/*   Updated: 2024/09/15 01:19:20 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	open_infile(t_pipex *pipex)
{
	char	*line;

	if (pipex->file_in_name == NULL)
		pipex->file_i = open(pipex->argv[1], O_RDONLY);
	else
	{
		pipex->file_i = open("here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (pipex->file_i < 0)
			error(pipex, "Couldn't open temporary file", 1);
		ft_printf(">");
		line = get_next_line(1);
		while (ft_strcmp(line, pipex->limiter) != 0)
		{
			write(pipex->file_i, line, ft_strlen(line));
			free(line);
			ft_printf(">");
			line = get_next_line(1);
		}
		free(line);
		close(pipex->file_i);
		pipex->file_i = open("here_doc", O_RDONLY);
	}
	if (pipex->file_i < 0)
		error(pipex, "Couldn't open input file", 1);
}

void	open_outfile(t_pipex *pipex)
{
	if (pipex->file_in_name == NULL)
		pipex->file_o = open(pipex->argv[(pipex->argc) - 1], O_CREAT | O_TRUNC
				| O_WRONLY, 0644);
	else
		pipex->file_o = open(pipex->argv[(pipex->argc) - 1], O_CREAT | O_APPEND
				| O_WRONLY, 0644);
	if (pipex->file_o < 0)
		error(pipex, "Couldn't open output file", 1);
}

void	free_all(t_pipex *pipex)
{
	int	i;

	if (pipex->pipe_fd[0] != -1)
		close(pipex->pipe_fd[0]);
	if (pipex->pipe_fd[1] != -1)
		close(pipex->pipe_fd[1]);
	if (pipex->prev_pipe_fd[0] != -1)
		close(pipex->prev_pipe_fd[0]);
	if (pipex->prev_pipe_fd[1] != -1)
		close(pipex->prev_pipe_fd[1]);
	if (pipex->file_i != -1)
		close(pipex->file_i);
	if (pipex->file_o != -1)
		close(pipex->file_o);
	i = -1;
	while (++i < pipex->n_cmd && pipex->pid[pipex->n_cmd - 1] != -1)
		waitpid(pipex->pid[i], &pipex->status, 0);
	free(pipex->pid);
	if (pipex->file_in_name)
		unlink(pipex->file_in_name);
	if (pipex->limiter)
		free(pipex->limiter);
}

void	error(t_pipex *pipex, char *message, int error_code)
{
	free_all(pipex);
	perror(message);
	exit(error_code);
}
