#include "minihell.h"

t_bool is_valid_command_format(t_cmd *cmd)
{
	int i = -1;
	t_cmd *current;

	current = cmd;
	printf("here?\n");
	if (!current)
		return FALSE;
	while(current)
	{
		if (!current->args)
			return TRUE;
		if (ft_strcmp(current->args[0], "|") == 0 || ft_strcmp(current->args[0], "||") == 0)
		{
			printf("bash: syntax error near unexpected token `%s'\n", current->args[0]);
			return FALSE;
		}
		while (current->args[++i])
		{
			if (ft_strcmp(current->args[i], ">") == 0 || ft_strcmp(current->args[i], ">>") == 0
			|| ft_strcmp(current->args[i], "<") == 0 || ft_strcmp(current->args[i], "<<") == 0)
			{
				if (!current->args[i + 1])
				{
					printf("bash: syntax error near unexpected token `newline'\n");
					return FALSE;
				}
			}
		}
		/* // Skip the first word (the command name)
		while (cmd[i] && !isspace(cmd[i]) && cmd[i] != '"' && cmd[i] != '\'' &&
			cmd[i] != '|' && cmd[i] != '>' && cmd[i] != '<')
			i++;

		if (cmd[i] && cmd[i] != ' ' && cmd[i] != '|' && cmd[i] != '>' 
			&& cmd[i] != '<' && cmd[i] != '\0')
			return FALSE; */
		current = current->next;
	}
	return TRUE;
}
/* 
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
		{
			perror("command not found: no path");
			exit(0);
		}
		if (execve(cmd->args[0], cmd->args, env->all) == -1)
		{
			i = -1;
			while (cmd->args[++i])
				free(cmd->args[i]);
			free(cmd->args);
			perror("command not found: no path");
			exit(127);
		}
	}
	waitpid(pid, 0, 0);
} */

void	print_cmd_list(t_cmd *cmd_lst)
{
	t_cmd	*current;
	int		index;

	current = cmd_lst;
	index = 0;
	while (current)
	{
		printf("Command Node %d\n", index);
		if (current->cmd)
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
		if (current->nb_infile)
		{
			printf("infile -> %d\n", current->nb_infile);
			int i = -1;
			while(current->in_file[++i])
				printf("  Input Redirection: %s\n", current->in_file[i]);
		}
		if (current->nb_outfile)
		{
			printf("outfile %d\n", current->nb_outfile);
			int i = -1;
			while(current->out_file[++i] && i < current->nb_outfile)
			{
				printf("  Output Redirection: %s\n",current->out_file[i]);
				printf("  Output Append ?%d\n", current->append[i]);
			}
		}
		current = current->next;
		index++;
	}
	printf("complete command print \n");
}

void	is_pipe(t_cmd *cmd)
{
	t_cmd *current;

	current = cmd;
	while (current)
	{
		if (current->next)
		{
			//printf("Pipe %s\n", current->cmd);
			current->is_pipe = TRUE;
		}
		else
			current->is_pipe = FALSE;
		current = current->next;
	}
}

void	ft_command(char *line, t_env *env)
{
	t_cmd	*commands;
	t_cmd	*tmp;

	commands = parse_command(line);
	(void) env;
	if (commands->cmd[0] == '\0')
		return;
	is_pipe(commands);
	print_cmd_list(commands);
	tmp = commands;
	execute_command(tmp, env);
	//while (waitpid(-1, NULL, 0) > 0); 
	free_cmd_list(commands);
}

void sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
}

void	set_signal_action(void)
{
	struct sigaction	act;

	ft_bzero(&act, sizeof(act));
	act.sa_handler = &sigint_handler;
	sigaction(SIGINT, &act, NULL);
	signal(SIGQUIT, SIG_IGN);
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

void minihell(t_env *env, int save_stdin, int save_stdout)
{
	//char	*prompt_hell_e;
	char	*line;
	int		i;

	i = 0;
	while (1)
	{
		dup2(save_stdin, STDIN_FILENO);
		dup2(save_stdout, STDOUT_FILENO);
		//prompt_hell_e = prompt_hell(i);
		/* if (i > 100) 
			explosion(); */
		//line = readline(prompt_hell_e);
		line = readline("\001\033[1;31m\002🔥 HellShell 🔥 \001\033[0m\002");
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
		//free(prompt_hell_e);
		free(line);
		i++;
	}
	exit(0);
}


int main(int argc, char **argv, char **environment)
{
	t_env	*env = NULL;
	int save_stdin;
	int save_stdout;

	(void)argc;
	(void)argv;
	save_stdin = dup(STDIN_FILENO);
	save_stdout = dup(STDOUT_FILENO);
	env = get_env(environment, env);
	/* signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, ignore_sigint); */
	set_signal_action();
	minihell(env, save_stdin, save_stdout);
	return (0);
}

/*
** TODO :
** - Handle errors during command execution.
** - Improve error handling.
*/