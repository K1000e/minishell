/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 00:23:06 by cgorin            #+#    #+#             */
/*   Updated: 2024/10/22 11:55:27 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <string.h>
# include <errno.h>
# include "../include/minihell.h"
# ifndef BONUS
#  define BONUS 0
# endif



typedef struct s_pipex
{
	pid_t	*pid;
	int		n_cmd;
	int		pipe_fd[2];
	int		prev_pipe_fd[2];
	char	*file_in_name;
	int		file_i;
	int		file_o;
	int		status;
	int		test;
	int		argc;
	char	**argv;
	char	*limiter;
	char	*outfile;
}	t_pipex;

/* FUNCTIONS */

// pipex_main.c
//int		pipex(t_cmd *cmd, t_env *env);
void	init_pipex(t_pipex *pipex);
void	ft_pipex(char **argv, char **env, t_pipex *pipex);
void	pipeline(char *cmd, char **env, t_pipex *pipex, int i);

// pipex_open_free.c
void	open_infile(t_pipex *pipex);
void	open_outfile(t_pipex *pipex);
void	free_all(t_pipex *pipex);
void	error(t_pipex *pipex, char *message, int error_code);

//pipex_execute.c
void	exec_pipe(t_pipex *pipex, char *cmd, char **env);
char	*find_executable(char *command, char **env);
char	*get_path_variable(char **env);

#endif