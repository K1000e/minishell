/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:57:23 by mabdessm          #+#    #+#             */
/*   Updated: 2024/12/01 23:40:58 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

char	*get_path_variable(t_env *env)
{
	t_env	*current;

	current = env->next;
	current = ft_find_key(current, "PATH");
	return(current->value + 1);
}


t_bool is_directory(const char *path)
{
	struct stat statbuf;
	if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
		return TRUE;
	return FALSE;
}

void find_executable(t_cmd *command, t_env *env)
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

void	fake_open_infile(t_pipex *pipex, t_cmd *cmd)
{
	int i;
	//char	*error;

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
	int i;

	i = -1;
	while (cmd->out_file && cmd->out_file[++i])
	{
		if (cmd->append[i] == 0)
			pipex->file_o = open(cmd->out_file[i], O_CREAT | O_TRUNC
					| O_WRONLY, 0644);
		else
			pipex->file_o = open(cmd->out_file[i], O_CREAT | O_APPEND
					| O_WRONLY, 0644);
		if (pipex->file_o < 0)
		{
			if (access(cmd->out_file[i], F_OK) == 0 && is_directory(cmd->out_file[i]))
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
	int i;

	i = -1;
	while (cmd->out_file && cmd->out_file[++i])
	{
		if (cmd->append[i] == 0)
			pipex->file_o = open(cmd->out_file[i], O_CREAT | O_TRUNC
					| O_WRONLY, 0644);
		else
			pipex->file_o = open(cmd->out_file[i], O_CREAT | O_APPEND
					| O_WRONLY, 0644);
		if (pipex->file_o < 0)
		{
			perror(cmd->out_file[i]);
			return(1);
		}
		if (i < cmd->nb_outfile - 1)
			close(pipex->file_o);
	}
	return (0);
}

int	open_infile(t_pipex *pipex, t_cmd *cmd)
{
	int i;

	i = -1;
	while (cmd->in_file && cmd->in_file[++i])
	{
		pipex->file_i = open(cmd->in_file[i], O_RDONLY);
		if (pipex->file_i < 0)
		{
			perror(cmd->in_file[i]);
			return(1);
		}
		if (i < cmd->nb_infile - 1)
			close(pipex->file_i);
	}
	return (0);
}

int	redirection_exec_bultins(t_cmd *cmd)
{
	t_pipex pipex;
	int exit_code;

	pipex.file_i = -1;
	pipex.file_o = -1;
	
	exit_code = 0;
	if (!cmd || !cmd->cmd)
		fake_error(&pipex, "Invalid command: command is empty or NULL", 1);
	if (cmd->nb_infile == 0)
 		dup2(pipex.pipe_fd[0], STDIN_FILENO);
 	else
	{
		exit_code += open_infile(&pipex, cmd);
		dup2(pipex.file_i, STDIN_FILENO);
	}
	if (cmd->nb_outfile == 0)
		dup2(pipex.pipe_fd[1], STDOUT_FILENO);
	else
	{
		exit_code += open_outfile(&pipex, cmd);
		dup2(pipex.file_o, STDOUT_FILENO);
	}
	if (pipex.pipe_fd[1] != -1)
		close(pipex.pipe_fd[1]);
	if (pipex.pipe_fd[0] != -1)
		close(pipex.pipe_fd[0]);
	return exit_code;
}

void execute_builtin(t_cmd *cmd, t_env *env)
{
	t_env *current;

	current = env;
	g_exit_code = 0;
	if (cmd->redirection)
		g_exit_code = redirection_exec_bultins(cmd);
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

char	*ft_join(char *buffer, char *buf)
{
	char	*tmp;

	tmp = ft_strjoin(buffer, buf);
	free(buffer);
	return (tmp);
}

void	execute_non_builtins(t_pipex *pipex, t_cmd *cmd, t_env *env, char **env_)
{
	cmd->path = TRUE;
	if (access(cmd->args[0], X_OK) != 0)
		find_executable(cmd, env);
	if (execve(cmd->args[0] , cmd->args, env_) == -1)
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
		else if (access(cmd->args[0], F_OK) == 0 && access(cmd->args[0], X_OK) != 0)
		{
			ft_fprintf(2, "bash: %s: Permission denied", cmd->args[0]);
			fake_error(pipex, "", 126);
		}
		else
		{
			ft_fprintf(2, "bash: %s: No such file or directory", cmd->args[0]);
			fake_error(pipex, "", 127);
		}
		//error = ft_strjoin("bash: ", cmd->args[0]);
		//error = ft_join(error, ": command not found");
		//fake_error(pipex, "command not found", 127);
	}
	if (!cmd->path)
	{
		ft_fprintf(2, "bash: %s: No such file or directory", cmd->args[0]);
		fake_error(pipex, "", 127);
	}
	exit(1);
	//g_exit_code = 1;
}

void	exec_non_builtins(t_cmd *cmd, t_env *env)
{
	t_pipex pipex;
	pid_t pid;
	char **b_env;

	b_env = base_env(env);
	g_exit_code = 0;
	pipex.status = 0;
	pipex.file_i = -1;
	pipex.file_o = -1;
	//pid = -1;
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
	else if (pid == -1)
	{
		for (int i = 0; b_env[i]; i++) ///?????
			free(b_env[i]);
		free(b_env);
		fake_error(&pipex, "Invalid fork()", 1);
	}
	for (int i = 0; b_env[i]; i++)
			free(b_env[i]);
		free(b_env);
	fake_free_all(&pipex);
	waitpid(pid, &g_exit_code, 0);
	g_exit_code = WEXITSTATUS(g_exit_code);
	//printf("%s : in exec exit code : %d\n",cmd->cmd, g_exit_code);
}

t_bool is_builtin(char *cmd)
{
	t_bool is_builtin;

	is_builtin = (
		ft_strcmp(cmd, "echo") == 0 ||
		ft_strcmp(cmd, "cd") == 0 ||
		ft_strcmp(cmd, "pwd") == 0 ||
		ft_strcmp(cmd, "env") == 0 ||
		ft_strcmp(cmd, "export") == 0 ||
		ft_strcmp(cmd, "unset") == 0 ||
		ft_strcmp(cmd, "exit") == 0
	);
	return (is_builtin);
}

/* void	parse_exec(t_cmd *cmd, t_env *env)
{
	if (!is_valid_command_format(cmd->cmd))
	{
		printf("Error: Invalid command format.\n");
		return ;
	}
	if (cmd->redirection)
		redirection_exec(cmd);
	if (cmd->args[0] && is_builtin(cmd->args[0]))
		execute_builtin(cmd, env);
	else 
		exec_non_builtins(cmd, env);
} */

void	execute_command(t_cmd *cmd ,t_env *env)
{
	pid_t child1;
	pid_t child2;
	int pipefd[2];

	if (!cmd->is_pipe)
	{
		if (!cmd->args[0] || is_builtin(cmd->args[0]))
			execute_builtin(cmd, env);
		else
			exec_non_builtins(cmd, env);
	}
	else
	{
		pipe(pipefd);
		child1 = fork();
		if (child1 == 0)
		{
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);
			cmd->is_pipe = FALSE;
			execute_command(cmd, env);
			exit(g_exit_code);
		}
		child2 = fork();
		if (child2 == 0)
		{
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);
			cmd = cmd->next;
			execute_command(cmd, env);
			exit(g_exit_code);
		}
		close(pipefd[0]);
		close(pipefd[1]);
		waitpid(child1, &g_exit_code, 0);
		waitpid(child2, &g_exit_code, 0);
		g_exit_code = WEXITSTATUS(g_exit_code);
	}
}