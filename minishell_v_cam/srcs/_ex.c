#include "minihell.h"

void exec_single_command(t_cmd *cmd, t_env *env)
{
	if (cmd->redirection)
		redirection_exec(cmd);
	if (cmd->args[0] && is_builtin(cmd->args[0]))
		execute_builtin(cmd, env);
	else
		exec_non_builtins(cmd, env);
}

void exec_pipe(t_cmd *cmd, t_env *env)
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
			parse_exec(cmd, env);
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