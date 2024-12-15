/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_exec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:57:23 by mabdessm          #+#    #+#             */
/*   Updated: 2024/12/15 01:56:58 by cgorin           ###   ########.fr       */
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

void handle_heredoc(char *delimiter, t_pipex *pipex, t_bool is_last)
{
	char	*line;
	unsigned long long		i;

	pipex->heredoc_file = ".heredoc";
	i = 0;
	while (access(pipex->heredoc_file, F_OK) == 0)
		pipex->heredoc_file = ft_strjoin(".heredoc", ft_itoa(i++));
	set_signal_action(sigint_heredoc_handler);
	pipex->heredoc_fd = open(pipex->heredoc_file, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	while (1)
	{
		ft_printf("> ");
		line = get_next_line(STDIN_FILENO);
		if (!line || ft_compare(delimiter, line))
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipex->heredoc_fd);
		free(line);
	}
	close(pipex->heredoc_fd);
	reopen_heredoc(pipex, is_last);
	set_signal_action(sigint_handler);
}

void	fake_open_infile(t_pipex *pipex, t_cmd *cmd)
{
	int	i;

	i = -1;
	while (cmd->in_file && cmd->in_file[++i])
	{
		pipex->file_i = open(cmd->in_file[i], O_RDONLY);
		if (pipex->file_i < 0)
			fake_error(pipex, "", 1);
		if (i < cmd->nb_infile - 1)
			close(pipex->file_i);
	}
}

void	fake_open_outfile(t_pipex *pipex, t_cmd *cmd)
{
	int	i;

	i = -1;
	while (cmd->out_file && cmd->out_file[++i])
	{
		if (cmd->append[i] == 0)
			pipex->file_o = open(cmd->out_file[i], O_CREAT | O_TRUNC | O_WRONLY,
					0644);
		else
			pipex->file_o = open(cmd->out_file[i],
					O_CREAT | O_APPEND | O_WRONLY, 0644);
		if (pipex->file_o < 0)
		{
			if (access(cmd->out_file[i], F_OK) == 0
				&& is_directory(cmd->out_file[i]))
			{
				ft_fprintf(2, "%s: Is a directory\n", cmd->out_file[i]);
				fake_error(pipex, "", 126);
			}
			else if (access(cmd->out_file[i], X_OK) == 0)
			{
				ft_fprintf(2, "%s: Permission denied\n", cmd->out_file[i]);
				fake_error(pipex, "", 126);
			}
			fake_error(pipex, "", 1);
		}
		if (i < cmd->nb_outfile - 1)
			close(pipex->file_o);
	}
}

int	open_infile(t_pipex *pipex, t_cmd *cmd, int j)
{
	int fd;

	fd = open(cmd->in_file[j], O_RDONLY);
	//printf("fd = %d\n", fd);
	if (fd < 0)
	{
		//printf("error\n");
		perror(cmd->in_file[j]);
		return(1);
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
	if (pipex->file_o)
		close(pipex->file_o);
	pipex->file_o = fd;
	return (0);
}

int	redirection_exec_bultins(t_cmd *cmd, t_pipex *pipex)
{
	int		i;
	int		j;
	int		k;
	int		h;

	if (cmd->order_file == NULL)
		return (1);
	i = -1;
	j = 0;
	k = 0;
	h = 0;
	while (cmd->order_file[++i])
	{
		if (cmd->order_file[i] == 'h')
		{
			if (h < cmd->nb_heredoc-1)
				handle_heredoc(cmd->heredoc_delimiter[h], pipex, FALSE);
			else
				handle_heredoc(cmd->heredoc_delimiter[h], pipex, TRUE);
			h++;
		}
		else if (cmd->order_file[i] == 'i')
		{
			if (open_infile(pipex, cmd, j) == 1)
				exit(1);
			j++;
		}
		else if (cmd->order_file[i] == 'o')
		{
			if (open_outfile(pipex, cmd, k) == 1)
				exit(1);
			k++;
		}
	}
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
	return (0);
}

int	redirection_exec_bultins_single(t_cmd *cmd, t_pipex *pipex)
{
	int		i;
	int		j;
	int		k;
	int		h;

	if (cmd->order_file == NULL)
		return (1);
	i = -1;
	j = 0;
	k = 0;
	h = 0;
	while (cmd->order_file[++i])
	{
		if (cmd->order_file[i] == 'h')
		{
			if (h < cmd->nb_heredoc-1)
				handle_heredoc(cmd->heredoc_delimiter[h], pipex, FALSE);
			else
				handle_heredoc(cmd->heredoc_delimiter[h], pipex, TRUE);
			h++;
		}
		else if (cmd->order_file[i] == 'i')
		{
			if (open_infile(pipex, cmd, j) == 1)
				return (1);
			/* fd = open(cmd->in_file[j], O_RDONLY);
			if (fd < 0)
			{
				perror(cmd->in_file[j]);
				return(1);
			}
			if (pipex->file_i != -1)
				close(pipex->file_i);
			pipex->file_i = fd; */
			j++;
		}
		else if (cmd->order_file[i] == 'o')
		{
			if (open_outfile(pipex, cmd, k) == 1)
				return (1);
			/* if (cmd->append[k] == 0)
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
			if (pipex->file_o)
				close(pipex->file_o);
			pipex->file_o = fd; */
			k++;
		}
		else
			return 1;
	}
	if (pipex->file_i != -1)
		dup2(pipex->file_i, STDIN_FILENO);
	if (pipex->file_i != -1 && cmd->heredoc_redirection)
		reopen_heredoc(pipex, FALSE);
	else if (pipex->file_i != -1)
		close(pipex->file_i);
	else if (pipex->file_i != -1)
		close(pipex->pipe_fd[1]);
	if (pipex->file_o != -1)
	{
		dup2(pipex->file_o, STDOUT_FILENO);
		close(pipex->file_o);
	}
	return (0);
}
