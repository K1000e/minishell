/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/16 18:37:53 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

void	execute_builtin(t_cmd *cmd, t_env *env, int single)
{
	t_env	*current;
	t_pipex	*pipex;

	pipex = malloc(sizeof(t_pipex));
	current = env;
	if (cmd->redirection)
		g_exit_code = redirection_exec_builtins(cmd, pipex, TRUE);
	if ((g_exit_code >= 1 && single) || cmd->args[0] == NULL)
		return ;
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		ft_exit(cmd);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		ft_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		ft_cd(cmd, env);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		ft_export(cmd, current);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		ft_unset(cmd, current);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		ft_pwd(env);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		ft_env(cmd, current);
	if (!single)
		free(pipex);
}

void	error(t_pipex *pipex, char *cmd, char *message, int error_code)
{
	ft_fprintf(2, "bash: %s: ", cmd);
	ft_fprintf(2, "%s\n", message);
	free_all(pipex);
	g_exit_code = error_code;
	exit(error_code);
}

void	execute_non_builtins(t_pipex *pipex, t_cmd *cmd, t_env *env,
		char **env_)
{
	struct stat	statbuf;

	cmd->path = TRUE;
	if (access(cmd->args[0], X_OK) != 0)
		find_executable(cmd, env);
	if (execve(cmd->args[0], cmd->args, env_) == -1)
	{
		if (ft_strchr(cmd->args[0], '/') && stat(cmd->args[0], &statbuf) == 0
			&& S_ISDIR(statbuf.st_mode))
			error(pipex, cmd->args[0], "Is a directory", 126);
		if (ft_strchr(cmd->args[0], '/') == NULL)
			error(pipex, cmd->args[0], "command not found", 127);
		else if (access(cmd->args[0], F_OK) == 0 && access(cmd->args[0],
				X_OK) != 0)
			error(pipex, cmd->args[0], "Permission denied", 126);
		else
			error(pipex, cmd->args[0], "No such file or directory", 127);
	}
	if (!cmd->path)
		error(pipex, cmd->args[0], "command not found", 127);
	exit(1);
}

void	exec_non_builtins(t_cmd *cmd, t_env *env)
{
	t_pipex	*pipex;
	pid_t	pid;
	char	**b_env;

	b_env = base_env(env);
	g_exit_code = 0;
	pipex = malloc(sizeof(t_pipex));
	pipex->file_i = -1;
	pipex->file_o = -1;
	if (pipe(pipex->pipe_fd) == -1)
		error(pipex, "", "Couldn't open pipe", 1);
	set_signal_action(sigint_heredoc_handler);
	pid = fork();
	if (pid == 0)
	{
		if (cmd->redirection)
			g_exit_code = redirection_exec_builtins(cmd, pipex, TRUE);
		execute_non_builtins(pipex, cmd, env, b_env);
		g_exit_code = WEXITSTATUS(waitpid(pid, &g_exit_code, 0));
	}
	set_signal_action(sigint_handler);
	free_string_array(b_env);
	free_all(pipex);
	waitpid(pid, &g_exit_code, 0);
	g_exit_code = WEXITSTATUS(g_exit_code);
}

t_bool	is_builtin(char *cmd)
{
	t_bool	is_builtin;

	is_builtin = (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
			|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "env") == 0
			|| ft_strcmp(cmd, "export") == 0 || ft_strcmp(cmd, "unset") == 0
			|| ft_strcmp(cmd, "exit") == 0);
	return (is_builtin);
}
