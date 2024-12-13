/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdessm <mabdessm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:01:24 by mabdessm          #+#    #+#             */
/*   Updated: 2024/11/27 17:22:44 by mabdessm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
	{
		ft_putchar_fd(s[i], fd);
		i++;
	}
}

int	ft_compare(char *limiter, char *line)
{
	if (!(ft_strncmp(line, limiter, ft_strlen(limiter)) == 0))
		return (0);
	if (line[ft_strlen(limiter)] != '\n')
		return (0);
	return (1);
}

void	reopen_heredoc(t_pipex *pipex)
{
	pipex->file_i = open(pipex->heredoc_file, O_RDONLY);
	if (pipex->file_i == -1)
	{
		perror("Opening Heredoc Failed");
		exit(EXIT_FAILURE);
	}
	if (unlink(".heredoc") == -1)
	{
		perror("Failed to remove heredoc file");
		exit(EXIT_FAILURE);
	}
}

void	heredoc(t_pipex *pipex, char *limiter)
{
	char	*line;
	int		heredoc_fd;

	pipex->heredoc_file = ".heredoc";
	heredoc_fd = open(pipex->heredoc_file, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (heredoc_fd == -1)
	{
		perror("Heredoc Creation Failed");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		ft_printf("> ");
		line = get_next_line(STDIN_FILENO);
		if (!line || ft_compare(limiter, line))
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, heredoc_fd);
		free(line);
	}
	close(heredoc_fd);
	reopen_heredoc(pipex);
}
