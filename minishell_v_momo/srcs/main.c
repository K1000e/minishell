#include "minihell.h"

t_bool is_valid_command_format(const char *cmd)
{
	int i = 0;

	// Skip the first word (the command name)
	while (cmd[i] && !isspace(cmd[i]) && cmd[i] != '"' && cmd[i] != '\'' &&
		   cmd[i] != '|' && cmd[i] != '>' && cmd[i] != '<')
		i++;

	if (cmd[i] && cmd[i] != ' ' && cmd[i] != '|' && cmd[i] != '>' 
		&& cmd[i] != '<' && cmd[i] != '\0')
		return FALSE;
	return TRUE;
}

void execute_builtin(t_cmd *cmd, t_env *env)
{
	t_env *current;

	current = env->next;
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		ft_exit(cmd);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		ft_echo(cmd);
	/* else if (ft_strcmp(cmd->args[0], "cd") == 0)
		ft_cd(cmd, env); */
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		ft_export(cmd, current);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		ft_unset(cmd, current);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		ft_env(cmd, current);
}

void execute_non_builtin(t_cmd *cmd, t_env *env)
{
	pid_t	pid;
	int		i;

	pid = fork();
	if (pid == 0)
	{
		if (access(cmd->args[0], X_OK) != 0)
			cmd->args[0] = find_executable(cmd->args[0], env);
		if (!cmd->cmd)
			printf("command not found: no path");
			//error(pipex, "command not found: no path", 0);
		if (execve(cmd->args[0], cmd->args, env->all) == -1)
		{
			printf("command not found");
			i = -1;
			while (cmd->args[++i])
				free(cmd->args[i]);
			free(cmd->args);
			//error(pipex, "command not found", 127);
		}
	}
	waitpid(pid, 0, 0);
}

void	ft_exec(t_cmd *cmd, t_env *env)
{
	int is_builtin;

	if (!is_valid_command_format(cmd->cmd))
	{
		printf("Error: Invalid command format.\n");
		return;
	}
	is_builtin = (
		ft_strcmp(cmd->args[0], "echo") == 0 ||
		ft_strcmp(cmd->args[0], "cd") == 0 ||
		ft_strcmp(cmd->args[0], "pwd") == 0 ||
		ft_strcmp(cmd->args[0], "env") == 0 ||
		ft_strcmp(cmd->args[0], "export") == 0 ||
		ft_strcmp(cmd->args[0], "unset") == 0 ||
		ft_strcmp(cmd->args[0], "exit") == 0
	);
	if (is_builtin)
		execute_builtin(cmd, env);  // Function to handle built-ins
	else
		execute_non_builtin(cmd, env);  // Function to exec non-built-in
}

void	print_cmd_list(t_cmd *cmd_lst)
{
	t_cmd	*current;
	int		index;

	current = cmd_lst;
	index = 0;
	while (current)
	{
		printf("Command Node %d:\n", index);
		printf("  Command: %s\n", current->cmd);
		if (current->args)
		{
			int i = 0;
			while (current->args[i])
			{
				printf("  Arguments[%d]: ", i);
				printf("%s \n", current->args[i]);
				i++;
			}
		}
		if (current->in_file)
			printf("  Input Redirection: %s\n", current->in_file);
		if (current->out_file)
			printf("  Output Redirection: %s append ?%d\n",
				current->out_file, current->append);
		current = current->next;
		index++;
	}
}

void	ft_command(char *line, t_env *env)
{
	t_cmd	*commands;
	t_cmd	*tmp;

	commands = parse_cmd(line);
	if (commands == NULL) {
		printf("Error: No commands were parsed.\n");
		return;
	}
	print_cmd_list(commands);
	tmp = commands;
	if (tmp->next)
		pipex(tmp, env); 
	else
		ft_exec(tmp, env);
	free_cmd_list(commands);
}

void ignore_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void explosion()
{
	printf(YELLOW "      ______________________________       . \\  | / .\n");
	printf("     /                              / \\     \\ \\ / /\n");
	printf("    |                              | ==========  - -\n");
	printf("     \\_____________________________\\_/     / / \\ \\ \n");
	printf("  _______________________________       \\  | / | \\ \n");
	printf(" /                               / \\     \\ \\ / /.   .\n");
	printf("|                               | ==========  - -\n");
	printf(" \\______________________________\\_/     / / \\ \\    /\n");
	printf("      ______________________________     / |\\  | /  .\n");
	printf("     /                              / \\     \\ \\ / /\n");
	printf("    |                               | ==========  -  - -\n");
	printf("     \\______________________________\\_/     / / \\ \\ \n");
	printf("                                         .  / | \\  .\n");
	printf( "🔥 Oh no ! The HellShell explodes! 🔥\n" RESET);
	rl_clear_history();
	exit(1);
}

void minihell(t_env *env)
{
	char	*prompt_hell_e;
	char	*line;
	int		i;

	i = 0;
	while (1)
	{
		prompt_hell_e = prompt_hell(i);
		if (i == 100) 
			explosion();
		line = readline(prompt_hell_e);
		if (line == NULL)
			break;
		add_history(line);
		if (!match_quotes(line) || !count_redir(line))
		{
			printf("Error\n");
			free(line);
			continue ;
		}
		ft_command(line, env);
		free (line);
		free(prompt_hell_e);
		i++;
	}
	exit(0);
}


int main(int argc, char **argv, char **environment)
{
	t_env	*env = NULL;
	
	(void)argc;
	(void)argv;

	env = get_env(environment, env);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, ignore_sigint);
	minihell(env);
	return (0);
}

/*
** TODO :
** - Handle errors during command execution.
** - Improve error handling.
*/