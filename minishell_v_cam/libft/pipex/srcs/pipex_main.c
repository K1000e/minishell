/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr				 +#+  +:+      +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 00:00:39 by cgorin			    #+#    #+#            */
/*   Updated: 2024/09/01 02:38:41 by cgorin			   ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **env)
{
	t_pipex	pipex;

	if ((argc == 5 && !BONUS) || (argc >= 5 && BONUS))
	{
		pipex.argc = argc;
		pipex.argv = argv;
		init_pipex(&pipex);
		if ((!BONUS && argc == 5) || (BONUS && argc >= 5
				&& ft_strcmp(argv[1], "here_doc") != 0))
			ft_pipex(argv, env, &pipex);
		else if (BONUS && argc >= 6 && ft_strcmp(argv[1], "here_doc") == 0)
			ft_pipex(argv + 1, env, &pipex);
		free_all(&pipex);
		if (errno == 0)
			exit(EXIT_SUCCESS);
		else
			exit(WEXITSTATUS(pipex.status));
	}
	else if (!BONUS)
		ft_printf("-> %s file1 cmd1 cmd2 file2\n", argv[0]);
	else if (BONUS)
		ft_printf("-> %s {file1}/{here_doc LIMITER} cmd1 .. cmd2 file2\n",
			argv[0]);
	exit(EXIT_FAILURE);
}

void	init_pipex(t_pipex *pipex)
{
	int	i;

	pipex->status = 0;
	if (ft_strcmp(pipex->argv[1], "here_doc") == 0)
	{
		pipex->file_in_name = pipex->argv[1];
		pipex->n_cmd = pipex->argc - 4;
		pipex->limiter = ft_strjoin(pipex->argv[2], "\n");
	}
	else
	{
		pipex->file_in_name = NULL;
		pipex->n_cmd = pipex->argc - 3;
		pipex->limiter = NULL;
	}
	pipex->pid = malloc(sizeof(pid_t) * (pipex->n_cmd));
	pipex->pipe_fd[0] = -1;
	pipex->pipe_fd[1] = -1;
	pipex->prev_pipe_fd[0] = -1;
	pipex->prev_pipe_fd[1] = -1;
	pipex->file_i = -1;
	pipex->file_o = -1;
	i = -1;
	while (++i < pipex->n_cmd)
		pipex->pid[i] = -1;
}

void	ft_pipex(char **argv, char **env, t_pipex *pipex)
{
	int	i;

	i = -1;
	while (++i < pipex->n_cmd)
	{
		if (i != pipex->n_cmd - 1)
			if (pipe(pipex->pipe_fd) == -1)
				error(pipex, "Couldn't open pipe", 1);
		pipex->pid[i] = fork();
		if (pipex->pid[i] == 0)
			pipeline(argv[i + 2], env, pipex, i);
		else if (pipex->pid[i] == -1)
			error(pipex, "Invalid fork()", 1);
		else
		{
			if (i > 0)
			{
				close(pipex->prev_pipe_fd[0]);
				close(pipex->prev_pipe_fd[1]);
			}
			pipex->prev_pipe_fd[0] = pipex->pipe_fd[0];
			pipex->prev_pipe_fd[1] = pipex->pipe_fd[1];
		}
	}
}

void	pipeline(char *cmd, char **env, t_pipex *pipex, int i)
{
	if (!cmd || !*cmd)
		error(pipex, "Invalid command: command is empty or NULL", 1);
	if (i == 0)
	{
		open_infile(pipex);
		dup2(pipex->file_i, STDIN_FILENO);
		dup2(pipex->pipe_fd[1], STDOUT_FILENO);
		close(pipex->pipe_fd[1]);
	}
	else if (i == pipex->n_cmd - 1)
	{
		open_outfile(pipex);
		close(pipex->pipe_fd[1]);
		dup2(pipex->prev_pipe_fd[0], STDIN_FILENO);
		dup2(pipex->file_o, STDOUT_FILENO);
		close(pipex->prev_pipe_fd[0]);
	}
	else
	{
		dup2(pipex->prev_pipe_fd[0], STDIN_FILENO);
		dup2(pipex->pipe_fd[1], STDOUT_FILENO);
		close(pipex->prev_pipe_fd[1]);
	}
	close(pipex->pipe_fd[0]);
	exec_pipe(pipex, cmd, env);
}
