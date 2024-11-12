#include "minihell.h"

char	*get_path_variable(t_env *env)
{
	t_env	*current;

	current = env->next;
	current = ft_find_key(current, "PATH");
	return(current->value + 1);
	/* while (current != NULL)
	{
		//if (env->key == NULL)
		if (ft_strcmp(current->key, "PATH") == 0)
		{
			return (current->value);
		}
		current = current->next;
	} */
}

char	*find_executable(char *command, t_env *env)
{
	size_t	i;
	char	*path;
	char	*dir;
	char	*full_path;
	int		start;

	path = get_path_variable(env);
	if (!path)
		return (NULL);
	start = 0;
	i = -1;
	while (++i <= ft_strlen(path))
	{
		if (path[i] == ':' || path[i] == '\0')
		{
			dir = ft_substr(path, start, i - start);
			full_path = ft_join(ft_join(dir, "/"), command);
			if (access(full_path, X_OK) == 0)
				return (full_path);
			free(full_path);
			start = i + 1;
		}
	}
	return (command);
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
	perror(message);
	exit(error_code);
}

void	fake_open_infile(t_pipex *pipex, t_cmd *cmd)
{
	int i;
	char	*error;

	i = -1;
	while (cmd->in_file && cmd->in_file[++i])
	{
		pipex->file_i = open(cmd->in_file[i], O_RDONLY);
		if (i < cmd->nb_infile - 1)
			close(pipex->file_i);
	}
	/* else
	{
		pipex->file_i = open("here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (pipex->file_i < 0)
			error(pipex, "Couldn't open temporary file", 1);
		ft_printf(">");
		line = get_next_line(1);
		while (ft_strcmp(line, pipex->limiter) != 0)
		{
			write(pipex->file_i, line, ft_strlen(line));
			free(line);
			ft_printf(">");
			line = get_next_line(1);
		}
		free(line);
		close(pipex->file_i);
		pipex->file_i = open("here_doc", O_RDONLY);
	} */
	if (pipex->file_i < 0)
	{
		//error = ft_strdup("bash: ");
		error = ft_strjoin("bash: ", cmd->in_file[cmd->nb_infile - 1]);
		fake_error(pipex, error, 1);
		free(error);
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
		if (i < cmd->nb_outfile - 1)
			close(pipex->file_o);
	}
	if (pipex->file_o < 0)
		fake_error(pipex, "Couldn't open output file", 1);
}

void	redirection_exec(t_cmd *cmd)
{
	t_pipex pipex;

	pipex.file_i = -1;
	pipex.file_o = -1;
	pipe(pipex.pipe_fd);
	//printf("redirect\n");
	if (!cmd || !cmd->cmd)
		fake_error(&pipex, "Invalid command: command is empty or NULL", 1);
	if (cmd->in_file)
	{
		fake_open_infile(&pipex, cmd);
		dup2(pipex.file_i, STDIN_FILENO);
	}
	/* else 
		dup2(pipex.pipe_fd[0], STDIN_FILENO); */
	if (cmd->out_file)
	{
		fake_open_outfile(&pipex, cmd);
		dup2(pipex.file_o, STDOUT_FILENO);
	}
	/* else
		dup2(pipex.pipe_fd[1], STDOUT_FILENO); */
	if (pipex.pipe_fd[1] != -1)
		close(pipex.pipe_fd[1]);
	if (pipex.pipe_fd[0] != -1)
		close(pipex.pipe_fd[0]);
	return ;
}

void execute_builtin(t_cmd *cmd, t_env *env)
{
	t_env *current;

	//printf("exec_builtins\n");
	current = env;
	/* if (cmd->redirection)
		redirection_exec(cmd); */
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

void	execute_non_builtins(t_pipex *pipex, t_cmd *cmd, t_env *env)
{
	int i;
	//printf("enter in execute_non_builtins\n");

	if (cmd->args[0])
	{
		if (access(cmd->args[0], X_OK) != 0)
			cmd->args[0] = find_executable(cmd->args[0], env);
		if (!cmd->args[0])
			fake_error(pipex, "command not found: no path", 0);
		if (execve(cmd->args[0] , cmd->args, env->all) == -1)
		{
			if (cmd->args)
			{
				i = -1;
				while (cmd->args[++i])
					free(cmd->args[i]);
				free(cmd->args);
			}
			if (cmd->out_file)
				free(cmd->out_file);
			else if (cmd->out_file)
				free(cmd->out_file);
			//fake_error(pipex, "command not found", 127);
		}
	}
}

void	exec_non_builtins(t_cmd *cmd, t_env *env)
{
	t_pipex pipex;
	pid_t pid;

	pipex.status = 0;
	pipex.file_i = -1;
	pipex.file_o = -1;
	pid = -1;

	//printf("exec_non_builtins (%s)\n", cmd->cmd);
	if (pipe(pipex.pipe_fd) == -1)
		fake_error(&pipex, "Couldn't open pipe", 1);
	pid = fork();
	if (pid == 0)
	{
		/* if (cmd->redirection)
			redirection_exec(cmd); */
		execute_non_builtins(&pipex, cmd, env);
		exit(0);
	}
	else if (pid == -1)
		fake_error(&pipex, "Invalid fork()", 1);
	fake_free_all(&pipex);
	/* waitpid(pid, NULL, 0);
	printf("test\n"); */
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

void	parse_exec(t_cmd *cmd, t_env *env)
{
	//printf("cmd = %s (parse_exec) -> pid : %d\n", cmd->cmd, getpid());
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
}


/*void	execute_command(t_cmd *cmd ,t_env *env)
{
	pid_t child1;
	int pipefd[2];

	printf("cmd = %s (execute_command)\n", cmd->cmd);
	if (!cmd->is_pipe)
	{
		printf("Enter in no pipe mode for %s\n", cmd->cmd);
		parse_exec(cmd, env);
	}
	else
	{
		printf("Enter in pipe mode for %s\n", cmd->cmd);
		if (pipe(pipefd) == -1)
			perror("Pipe failed");
		child1 = fork();
		if (child1 == -1)
			perror("Fork failed");
		if (child1 == 0)
		{
			printf("Child process for %s -> pid: %d\n", cmd->cmd, getpid());
			close(pipefd[0]);
			if (dup2(pipefd[1], STDOUT_FILENO) == -1)
			{
				perror("Duplication to STDOUT failed");
				exit(1);
			}
			close(pipefd[1]);
			parse_exec(cmd, env);
			printf("Child exiting for %s -> pid: %d\n", cmd->cmd, getpid());
			exit(0);
		}
		else
		{
			printf("Parent process waiting -> pid: %d, child pid: %d\n", getpid(), child1);
			close(pipefd[1]);
			if (dup2(pipefd[0], STDIN_FILENO) == -1)
			{
				perror("Duplication to STDOUT failed");
				exit(1);
			}
			close(pipefd[0]);
			waitpid(child1, NULL, 0);
			if (cmd->next)
				execute_command(cmd->next, env);
		}
	}
} */

void exec_single_command(t_cmd *cmd, t_env *env)
{
	if (cmd->redirection)
		redirection_exec(cmd);
	if (cmd->args[0] && is_builtin(cmd->args[0]))
		execute_builtin(cmd, env);
	else
		exec_non_builtins(cmd, env);
}

void exec_pipe_(t_cmd *cmd, t_env *env)
{
	t_pipex pipex;
	pid_t child;
	int input_fd = STDIN_FILENO;

	while(cmd)
	{
		pipe(pipex.pipe_fd);
		child = fork();
		if (child  == 0)
		{
			dup2(input_fd, STDIN_FILENO);
			if (cmd->next)
				dup2(pipex.pipe_fd[1], STDOUT_FILENO);
			close(pipex.pipe_fd[0]);
			exec_single_command(cmd, env);
			exit(0);
		}
		else
		{
			waitpid(child, NULL, 0);
			close(pipex.pipe_fd[1]);
			input_fd = pipex.pipe_fd[0];
			cmd = cmd->next;
		}
	}
}

void	execute_command(t_cmd *cmd ,t_env *env)
{
	if (!cmd->is_pipe)
		exec_single_command(cmd, env);
	else
		exec_pipe_(cmd, env);
}
