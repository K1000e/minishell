/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:57:23 by mabdessm          #+#    #+#             */
/*   Updated: 2024/12/12 16:40:53 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

char	*get_path_variable(t_env *env)
{
	t_env	*current;

	current = env->next;
	current = ft_find_key(current, "PATH");
	if (current)
		return (current->value);
	else
		return (NULL);
}

t_bool	is_directory(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
		return (TRUE);
	return (FALSE);
}

void	find_executable(t_cmd *command, t_env *env)
{
	size_t	i;
	char	*path;
	char	*dir;
	char	*full_path;
	int		start;

	path = get_path_variable(env);
	if (!path)
	{
		command->path = FALSE;
		return ;
	}
	start = 0;
	i = -1;
	while (++i <= ft_strlen(path))
	{
		if (path[i] == ':' || path[i] == '\0')
		{
			dir = ft_substr(path, start, i - start);
			full_path = ft_join(ft_join(dir, "/"), command->args[0]);
			if (access(full_path, X_OK) == 0)
			{
				command->args[0] = ft_strdup(full_path);
				free(full_path);
				command->path = TRUE;
				return ;
			}
			free(full_path);
			start = i + 1;
		}
	}
	command->path = FALSE;
}

void	fake_free_all(t_pipex *pipex)
{
	if (pipex->pipe_fd[0] != -1)
		close(pipex->pipe_fd[0]);
	if (pipex->pipe_fd[1] != -1)
		close(pipex->pipe_fd[1]);
	if (pipex->file_i != -1)
		close(pipex->file_i);
	if (pipex->file_o != -1)
		close(pipex->file_o);
}

void	fake_error(t_pipex *pipex, char *message, int error_code)
{
	fake_free_all(pipex);
	ft_fprintf(2, "%s\n", message);
	g_exit_code = error_code;
	exit(error_code);
}

void	single_builtin(t_cmd *cmd, t_env *env)
{
	t_env	*current;

	current = env;
	g_exit_code = 0;
	if (cmd->redirection)
		g_exit_code = redirection_exec_bultins_single(cmd);
	if (g_exit_code >= 1)
	{
		g_exit_code = 1;
		return ;
	}
	if (cmd->args[0] == NULL)
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
}

void	execute_builtin(t_cmd *cmd, t_env *env)
{
	t_env	*current;

	current = env;
	g_exit_code = 0;
	if (cmd->redirection)
		g_exit_code = redirection_exec_bultins(cmd);
	if (cmd->args[0] == NULL)
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
}

char	*ft_join(char *buffer, char *buf)
{
	char	*tmp;

	if (!buffer)
		return (ft_strdup(buf));
	tmp = ft_strjoin(buffer, buf);
	free(buffer);
	return (tmp);
}

void	execute_non_builtins(t_pipex *pipex, t_cmd *cmd, t_env *env,
		char **env_)
{
	cmd->path = TRUE;
	if (access(cmd->args[0], X_OK) != 0)
		find_executable(cmd, env);
	if (execve(cmd->args[0], cmd->args, env_) == -1)
	{
		if (ft_strchr(cmd->args[0], '/') && is_directory(cmd->args[0]))
		{
			ft_fprintf(2, "%s: Is a directory", cmd->args[0]);
			fake_error(pipex, "", 126);
		}
		if (ft_strchr(cmd->args[0], '/') == NULL)
		{
			ft_fprintf(2, "bash: %s: command not found", cmd->args[0]);
			fake_error(pipex, "", 127);
		}
		else if (access(cmd->args[0], F_OK) == 0 && access(cmd->args[0],
				X_OK) != 0)
		{
			ft_fprintf(2, "bash: %s: Permission denied", cmd->args[0]);
			fake_error(pipex, "", 126);
		}
		else
		{
			ft_fprintf(2, "bash: %s: No such file or directory", cmd->args[0]);
			fake_error(pipex, "", 127);
		}
	}
	if (!cmd->path)
	{
		ft_fprintf(2, "bash: %s: No such file or directory", cmd->args[0]);
		fake_error(pipex, "", 127);
	}
	exit(1);
}

void	exec_non_builtins(t_cmd *cmd, t_env *env)
{
	t_pipex	pipex;
	pid_t	pid;
	int		i;
	char	**b_env;

	b_env = base_env(env);
	g_exit_code = 0;
	pipex.file_i = -1;
	pipex.file_o = -1;
	if (pipe(pipex.pipe_fd) == -1)
		fake_error(&pipex, "Couldn't open pipe", 1);
	pid = fork();
	if (pid == 0)
	{
		if (cmd->redirection)
			g_exit_code = redirection_exec_bultins(cmd);
		if (g_exit_code >= 1)
			exit(1);
		execute_non_builtins(&pipex, cmd, env, b_env);
		waitpid(pid, &g_exit_code, 0);
		g_exit_code = WEXITSTATUS(g_exit_code);
	}
	i = 0;
	while (b_env[i])
		free(b_env[i++]);
	free(b_env);
	if (pid == -1)
		fake_error(&pipex, "Invalid fork()", 1);
	fake_free_all(&pipex);
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

void	ex_child1(int pipefd[2], t_cmd *cmd, t_env *env)
{
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	cmd->is_pipe = FALSE;
	execute_command(cmd, env);
	exit(g_exit_code);
}

void	ex_child2(int pipefd[2], t_cmd *cmd, t_env *env)
{
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	cmd = cmd->next;
	execute_command(cmd, env);
	exit(g_exit_code);
}

void	parent(pid_t child1, pid_t child2, int pipefd[2])
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
			execute_builtin(cmd, env);
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
