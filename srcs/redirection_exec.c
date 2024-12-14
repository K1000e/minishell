/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_exec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:57:23 by mabdessm          #+#    #+#             */
/*   Updated: 2024/12/14 16:08:36 by cgorin           ###   ########.fr       */
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
	pipex->file_i = open(pipex->heredoc_file, O_RDONLY);
	if (pipex->file_i == -1)
	{
		perror("Opening Heredoc Failed");
	//	exit(EXIT_FAILURE);
	}
	if (is_last)
	{
		if (unlink(pipex->heredoc_file) == -1)
		{
			perror("Failed to remove heredoc file");
		//	exit(EXIT_FAILURE);
		}
	}
}

int handle_heredoc(char *delimiter, t_bool is_last)
{
	char	*line;
	//int		heredoc_fd;
	t_pipex	*pipex;
	//pid_t pid;
	unsigned long long		i;

	pipex = malloc(sizeof(t_pipex));
	pipex->file_i = -1;
	pipex->file_o = -1;
	pipex->heredoc_file = ".heredoc";
	i = 0;
	while (access(pipex->heredoc_file, F_OK) == 0)
		pipex->heredoc_file = ft_strjoin(".heredoc", ft_itoa(i++));
	pipex->heredoc_fd = open(pipex->heredoc_file, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (pipex->heredoc_fd == -1)
	{
		perror("Heredoc Creation Failed");
		exit(EXIT_FAILURE);
	}
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
	return (pipex->heredoc_fd);
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

int	open_outfile(t_pipex *pipex, t_cmd *cmd)
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
			perror(cmd->out_file[i]);
			return (1);
		}
		if (i < cmd->nb_outfile - 1)
			close(pipex->file_o);
	}
	return (0);
}

int	open_infile(t_pipex *pipex, t_cmd *cmd)
{
	int	i;

	i = -1;
	while (cmd->in_file && cmd->in_file[++i])
	{
		pipex->file_i = open(cmd->in_file[i], O_RDONLY);
		if (pipex->file_i < 0)
		{
			perror(cmd->in_file[i]);
			return (1);
		}
		if (i < cmd->nb_infile - 1)
			close(pipex->file_i);
	}
	return (0);
}

int	redirection_exec_bultins(t_cmd *cmd)
{
	t_pipex	pipex;
	int		exit_code;
	int		i;
	int		j;
	int		k;

	pipex.file_i = -1;
	pipex.file_o = -1;
	pipex.pipe_fd[0] = -1;
	pipex.pipe_fd[1] = -1;
	exit_code = 0;
	if (!cmd || !cmd->cmd)
		fake_error(&pipex, "Invalid command: command is empty or NULL", 1);
	i = -1;
	j = 0;
	k = 0;
	int h = 0;
	/* if (!cmd->order_file && (int)ft_strlen(cmd->order_file) != cmd->nb_infile + cmd->nb_outfile + cmd->nb_heredoc)
	{
		ft_fprintf(stderr, "Error: Invalid redirection metadata\n");
		return 1;
	} */
	while ((size_t)++i < ft_strlen(cmd->order_file))
	{
		if (cmd->order_file[i] == 'h')
		{
			//pipex.file_i = handle_heredoc(cmd->heredoc_delimiter[h]);
			if (i < cmd->nb_infile + cmd->nb_heredoc)
				pipex.file_i = handle_heredoc(cmd->heredoc_delimiter[h], FALSE);
			else
				pipex.file_i = handle_heredoc(cmd->heredoc_delimiter[h], TRUE);
			if (pipex.file_i < 0)
				exit(1);
			if (i < cmd->nb_infile + cmd->nb_heredoc - 1)
				close(pipex.file_i);
			h++;
		}
		else if (cmd->order_file[i] == 'i')
		{
			pipex.file_i = open(cmd->in_file[j], O_RDONLY);
			if (pipex.file_i < 0)
			{
				perror(cmd->in_file[j]);
				exit(1);
			}
			if (i < cmd->nb_infile - 1 + cmd->nb_heredoc - 1)
				close(pipex.file_i);
			j++;
		}
		else if (cmd->order_file[i] == 'o')
		{
			if (cmd->append[k] == 0)
				pipex.file_o = open(cmd->out_file[k],
						O_CREAT | O_TRUNC | O_WRONLY, 0644);
			else
				pipex.file_o = open(cmd->out_file[k],
						O_CREAT | O_APPEND | O_WRONLY, 0644);
			if (pipex.file_o < 0)
			{
				perror(cmd->out_file[k]);
				exit(1);
			}
			if (i < cmd->nb_outfile - 1)
				close(pipex.file_o);
			k++;
		}
	}
	if (cmd->nb_infile == 0)
		dup2(pipex.pipe_fd[0], STDIN_FILENO);
	else
		dup2(pipex.file_i, STDIN_FILENO);
	if (cmd->nb_outfile == 0)
		dup2(pipex.pipe_fd[1], STDOUT_FILENO);
	else
		dup2(pipex.file_o, STDOUT_FILENO);
	if (pipex.pipe_fd[1] != -1)
		close(pipex.pipe_fd[1]);
	if (pipex.pipe_fd[0] != -1)
		close(pipex.pipe_fd[0]);
	return (exit_code);
}

int	redirection_exec_bultins_single(t_cmd *cmd)
{
	t_pipex	pipex;
	int		i;
	int		j;
	int		k;
	int fd;

	pipex.file_i = -1;
	pipex.file_o = -1;
	if (!cmd || !cmd->cmd)
		fake_error(&pipex, "Invalid command: command is empty or NULL", 1);
	i = -1;
	j = 0;
	k = 0;
	int h = 0;
	while ((size_t)++i < ft_strlen(cmd->order_file))
	{
		if (cmd->order_file[i] == 'h')
		{
			//pipex.file_i = handle_heredoc(cmd->heredoc_delimiter[h]);
			if (i < cmd->nb_infile + cmd->nb_heredoc)
				pipex.file_i = handle_heredoc(cmd->heredoc_delimiter[h], FALSE);
			else
				pipex.file_i = handle_heredoc(cmd->heredoc_delimiter[h], TRUE);
			if (fd < 0)
				return(1);
			if (pipex.file_i != -1)
				close(pipex.file_i);
			pipex.file_i = fd;
			h++;
		}
		else if (cmd->order_file[i] == 'i')
		{
			fd = open(cmd->in_file[j], O_RDONLY);
			if (fd < 0)
			{
				perror(cmd->in_file[j]);
				return(1);
			}
			if (pipex.file_i != -1)
				close(pipex.file_i);
			pipex.file_i = fd;
			j++;
		}
		else if (cmd->order_file[i] == 'o')
		{
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
			if (pipex.file_o)
				close(pipex.file_o);
			pipex.file_o = fd;
			k++;
		}
		else
		{
			ft_fprintf(2, "Error: Unknown redirection type '%c'\n", cmd->order_file[i]);
			return 1;
		}
	}
	if (pipex.file_i != -1)
	{
		dup2(pipex.file_i, STDIN_FILENO);
		close(pipex.file_i);
	}
	if (pipex.file_o != -1)
	{
		dup2(pipex.file_o, STDOUT_FILENO);
		close(pipex.file_o);
	}
	return (0);
}