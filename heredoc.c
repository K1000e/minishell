/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 23:31:25 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/15 03:15:34 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

int	ft_compare(char *limiter, char *line)
{
	if (!(ft_strncmp(line, limiter, ft_strlen(limiter)) == 0))
		return (0);
	if (line[ft_strlen(limiter)] != '\n')
		return (0);
	return (1);
}

void	reopen_heredoc(t_pipex *pipex, t_bool is_last)
{
	if (is_last)
		pipex->file_i = open(pipex->heredoc_file, O_RDONLY);
	if (pipex->file_i == -1)
		perror("Opening Heredoc Failed");
	if (!is_last)
		if (unlink(pipex->heredoc_file) == -1)
			perror("Failed to remove heredoc file");
}

void	handle_heredoc(char *delimiter, t_pipex *pipex, t_bool is_last)
{
	char				*line;
	unsigned long long	i;

	pipex->heredoc_file = ".heredoc";
	i = 0;
	while (access(pipex->heredoc_file, F_OK) == 0)
		pipex->heredoc_file = ft_strjoin(".heredoc", ft_itoa(i++));
	set_signal_action(sigint_heredoc_handler);
	pipex->heredoc_fd = open(pipex->heredoc_file,
			O_CREAT | O_WRONLY | O_TRUNC, 0777);
	while (1)
	{
		ft_printf("> ");
		line = get_next_line(STDIN_FILENO);
		if (!line || ft_compare(delimiter, line))
		{
			free(line);
			break ;
		}
		ft_fprintf(pipex->heredoc_fd, "%s", line);
		free(line);
	}
	close(pipex->heredoc_fd);
	reopen_heredoc(pipex, is_last);
	set_signal_action(sigint_handler);
}
