/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 23:31:25 by cgorin            #+#    #+#             */
/*   Updated: 2025/01/06 19:45:22 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_compare(char *limiter, char *line, int index)
{
	if (!line)
	{
		ft_fprintf(2, "HellShell: warning: here-document at line %d", index);
		ft_fprintf(2, " delimited by end-of-file (wanted `%s')\n", limiter);
		return (1);
	}
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
	if (!is_last)
		if (unlink(pipex->heredoc_file) == -1)
			perror("Failed to remove heredoc file");
	if (!is_last && pipex->heredoc_file)
		free(pipex->heredoc_file);
}

static void	heredoc_loop(char *delimiter, char *line, t_pipex *pipex)
{
	int	i;

	i = 1;
	while (i++)
	{
		ft_fprintf(0, "> ");
		line = get_next_line(0);
		if (ft_compare(delimiter, line, i))
		{
			free(line);
			break ;
		}
		ft_fprintf(pipex->heredoc_fd, "%s", line);
		free(line);
	}
}

void	handle_heredoc(char *delimiter, t_pipex *pipex, t_bool is_last)
{
	char				*line;
	char				*tmp;
	unsigned long long	i;

	pipex->heredoc_file = ft_strdup(".heredoc");
	i = 0;
	while (access(pipex->heredoc_file, F_OK) == 0)
	{
		free(pipex->heredoc_file);
		pipex->heredoc_file = NULL;
		tmp = ft_itoa(i++);
		pipex->heredoc_file = ft_strjoin(".heredoc", tmp);
		free(tmp);
	}
	set_signal_action(sigint_heredoc_handler);
	pipex->heredoc_fd = open(pipex->heredoc_file,
			O_CREAT | O_WRONLY | O_TRUNC, 0777);
	line = NULL;
	heredoc_loop(delimiter, line, pipex);
	close(pipex->heredoc_fd);
	reopen_heredoc(pipex, is_last);
	set_signal_action(sigint_handler);
}
