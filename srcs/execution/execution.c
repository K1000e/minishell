/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/16 22:34:59 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

static void	ex_child1(int pipefd[2], t_cmd *cmd, t_env *env)
{
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	cmd->is_pipe = FALSE;
	execute_command(cmd, env);
	exit(g_exit_code);
}

static void	ex_child2(int pipefd[2], t_cmd *cmd, t_env *env)
{
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	cmd = cmd->next;
	execute_command(cmd, env);
	exit(g_exit_code);
}

static void	parent(pid_t child1, pid_t child2, int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(child1, &g_exit_code, 0);
	waitpid(child2, &g_exit_code, 0);
	g_exit_code = WEXITSTATUS(g_exit_code);
}

void	execute_command(t_cmd *cmd, t_env *env)
{
	pid_t	child1;
	pid_t	child2;
	int		pipefd[2];

	if (!cmd->is_pipe)
	{
		if (!cmd->args[0] || is_builtin(cmd->args[0]))
			execute_builtin(cmd, env, FALSE);
		else
			exec_non_builtins(cmd, env);
		return ;
	}
	pipe(pipefd);
	child1 = fork();
	if (child1 == 0)
		ex_child1(pipefd, cmd, env);
	child2 = fork();
	if (child2 == 0)
		ex_child2(pipefd, cmd, env);
	parent(child1, child2, pipefd);
}
