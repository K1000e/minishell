/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   main.c											 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: cgorin <cgorin@student.42.fr				 +#+  +:+	  +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/07/15 00:00:39 by cgorin				#+#	#+#			*/
/*   Updated: 2024/09/01 02:38:41 by cgorin			   ###   ########.fr	  */
/*																			*/
/* ************************************************************************** */

#include "minihell.h"
#include "pipex.h"

int	ft_pipex_start(t_cmd *cmd, t_env *env)
{
	t_pipex	pipex;

	/* if ((argc == 5 && !BONUS) || (argc >= 5 && BONUS))
	{ */
		/* pipex.argc = argc;
		pipex.argv = argv; */
		init_ft_pipex_start(&pipex, cmd);
		
		/* if ((!BONUS && argc == 5) || (BONUS && argc >= 5
				&& ft_strcmp(argv[1], "here_doc") != 0)) */
		ft_ft_pipex_start(cmd, env, &pipex);
		/* else if (BONUS && argc >= 6 && ft_strcmp(argv[1], "here_doc") == 0)
			ft_ft_pipex_start(argv + 1, env, &pipex); */
		free_all(&pipex);
		/* if (errno == 0)
			exit(EXIT_SUCCESS);
		else
			exit(WEXITSTATUS(pipex.status)); */
	//}
	/* else if (!BONUS)
		ft_printf("-> %s file1 cmd1 cmd2 file2\n", argv[0]);
	else if (BONUS)
		ft_printf("-> %s {file1}/{here_doc LIMITER} cmd1 .. cmd2 file2\n",
			argv[0]);
	exit(EXIT_FAILURE); */
	return 0;
}

void	init_ft_pipex_start(t_pipex *pipex, t_cmd *cmd)
{
	int	i;
	t_cmd *current;
	//int 

	current = cmd;
	pipex->status = 0;
	/* if (ft_strcmp(pipex->argv[1], "here_doc") == 0)
	{
		pipex->file_in_name = pipex->argv[1];
		pipex->n_cmd = pipex->argc - 4;
		pipex->limiter = ft_strjoin(pipex->argv[2], "\n");
		printf(RED"there\n\033[0m"RESET);
	}
	else
	{ */
	i = 0;
	while (current)
	{
		i++;
		current = current->next;
	}
	printf("nb cmd : %d\n", i);
	//pipex->file_in_name = NULL;
	pipex->n_cmd = i;
	//pipex->limiter = NULL;
	//}
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

void ft_ft_pipex_start(t_cmd *cmd, t_env *env, t_pipex *pipex)
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
			pipeline(cmd, env, pipex, i);
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
/*     int i = 0;
    while (++i < pipex->n_cmd && cmd[i].cmd != NULL) {
        // Set up pipes and fork for each command in the pipeline
        pipeline(cmd[i].cmd, *env, pipex, i);
    } */
}

void	pipeline(t_cmd *cmd, t_env *env, t_pipex *pipex, int i)
{
	if (!cmd || !cmd->cmd)
		error(pipex, "Invalid command: command is empty or NULL", 1);
	if (i == 0)
	{
		printf("wtf1\n");
		printf("in_file1 = %s", cmd->in_file);
		printf("out_file1 = %s", cmd->out_file);
		if (cmd->in_file)
		{
			open_infile(pipex);
			dup2(pipex->file_i, STDIN_FILENO);
		}
		else 
			dup2(pipex->pipe_fd[0], STDIN_FILENO);
		if (cmd->out_file)
		{
			open_outfile(pipex);
			dup2(pipex->file_o, STDOUT_FILENO);
		}
		else
			dup2(pipex->pipe_fd[1], STDOUT_FILENO);
		close(pipex->pipe_fd[1]);
	}
	else if (i == pipex->n_cmd - 1)
	{
		printf("wtf2\n");
		printf("in_file2 = %s", cmd->in_file);
		printf("out_file2 = %s", cmd->out_file);
		if (cmd->in_file)
		{
			open_infile(pipex);
			dup2(pipex->file_i, STDIN_FILENO);
		}
		else
			dup2(pipex->prev_pipe_fd[0], STDIN_FILENO);
		if (cmd->out_file)
		{
			open_outfile(pipex);
			dup2(pipex->file_o, STDOUT_FILENO);
		}
		else 
			dup2(pipex->pipe_fd[1], STDOUT_FILENO);
		close(pipex->pipe_fd[1]);
		close(pipex->prev_pipe_fd[0]);
	}
	else
	{
		printf("wtf3\n");
		printf("in_file3 = %s", cmd->in_file);
		printf("out_file3 = %s", cmd->out_file);
		if (cmd->in_file)
		{
			open_infile(pipex);
			dup2(pipex->file_i, STDIN_FILENO);
		}
		else
			dup2(pipex->prev_pipe_fd[0], STDIN_FILENO);
		if (cmd->out_file)
		{
			open_outfile(pipex);
			dup2(pipex->file_o, STDOUT_FILENO);
		}
		else 
			dup2(pipex->pipe_fd[1], STDOUT_FILENO);
		close(pipex->prev_pipe_fd[1]);
	}
	//printf("exit\n");
	close(pipex->pipe_fd[0]);
	exec_pipe(pipex, cmd, env);
}
