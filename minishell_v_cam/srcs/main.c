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
/*

t_bool is_valid_command_format(t_cmd *cmd)
{
	int i = -1;
	t_cmd *current;

	current = cmd;
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
		current = current->next;
	}
	return TRUE;
}
*/
/* 
void execute_non_builtin(t_cmd *cmd, t_env *env)
{
	pid_t	pid;🔥 HellShell 🔥
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
			current->is_pipe = TRUE;
		}
		else
			current->is_pipe = FALSE;
		current = current->next;
	}
}

char	*get_env_var_value(const char *var_name, t_env *env)
{
	t_env	*current;

	current = env->next;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

char	*expand_env_vars(const char *input, t_env *env)
{
	char		*result;
	char		*res_ptr;
	const char	*ptr;
	int			i;
	char		*value;
	char 		*var_name;

	result = malloc(sizeof(char) * (ft_strlen(input) + 1));
	res_ptr = result;
	ptr = input;
	var_name = ft_calloc(sizeof(char), ft_strlen(input) + 1);
	while (*ptr)
	{
		if (*ptr == '$' && (ft_isalpha(ptr[1]) || ptr[1] == '_'))
		{
			i = 0;
			++ptr;
			while (ft_isalnum(*ptr) || *ptr == '_')
			{
				var_name[i] = *ptr;
				++i;
				++ptr;
			}
			var_name[i] = '\0';
			value = get_env_var_value(var_name, env);
			if (value)
			{
				value++;
				ft_strcpy(res_ptr, value);
				res_ptr += strlen(value);
			}
			++ptr;
		}
		else if (*ptr == '$' && (ft_isdigit(ptr[1])))
		{
			++ptr;
			if (*ptr == '0')
			{
				ft_strcpy(res_ptr, "executable");
				res_ptr += 4;
			}
			++ptr;
		}
		else if (*ptr == '$' && ptr[1] == '?')
		{
			++ptr;
			if (*ptr == '0')
			{
				ft_strcpy(res_ptr, "exit_code");
				res_ptr += 4;
			}
			++ptr;
		}
		else
		{
			*res_ptr = *ptr;
			++res_ptr;
			++ptr;
		}
	}
	*res_ptr = '\0';
	free(var_name);
	return (result);
}

/* char	*expand_env_vars(const char *input, t_env *env)
{
	char		*result;
	char		*res_ptr;
	//const char	*ptr;
	int			i;
	int			j;
	char		*value;
	char 		*var_name;

	result = malloc(sizeof(char) * (ft_strlen(input) + 1));
	res_ptr = result;
	//ptr = input;
	var_name = ft_calloc(sizeof(char), ft_strlen(input) + 1);
	i = -1;
	while (input[i])
	{
		if (input[i] == '$' && (ft_isalpha(input[i + 1]) || input[i + 1] == '_'))
		{
			j = 0;
			while (ft_isalnum(input[j]) || input[j] == '_')
			{
				var_name[j] = input[i];
				++j;
				++i;
			}
			var_name[j] = '\0';
			value = get_env_var_value(var_name, env);
			printf("var_name = %s & value = %s\n", var_name, value);
			if (value)
			{
				ft_strcpy(res_ptr, value);
				res_ptr += strlen(value);
			}
			++i;
		}
		else
		{
			*res_ptr = input[i];
			++res_ptr;
			++i;
		}
	}
	*res_ptr = '\0';
	free(var_name);
	return (result);
} */

void	ft_command(char *line, t_env *env)
{
	char	*expanded_line;
	t_cmd	*commands;
	t_cmd	*tmp;

	expanded_line = expand_env_vars(line, env);
	commands = parse_command(expanded_line);
	(void) env;
	free(expanded_line);
	if (commands->cmd[0] == '\0')
		return;
	is_pipe(commands);
	print_cmd_list(commands);
	tmp = commands;
	execute_command(tmp, env);
	free_cmd_list(commands);
}

void	sigint_handler(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sigint_heredoc_handler(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		close(STDIN_FILENO);
	}
}

void	set_signal_action(void (*handler)(int))
{
	struct sigaction	act;

	ft_bzero(&act, sizeof(act));
	act.sa_handler = handler;
	sigaction(SIGINT, &act, NULL);
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
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
	//int		i;

	//i = 0;
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
			if (ft_strlen(line) > 0)
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
	}
	exit(0);
}

int g_exit_code = 0;

int	main(int argc, char **argv, char **environment)
{
	t_env	*env;
	int save_stdin;
	int save_stdout;

	env = NULL;
	(void)argc;
	(void)argv;
	save_stdin = dup(STDIN_FILENO);
	save_stdout = dup(STDOUT_FILENO);
	env = get_env(environment, env);
	set_signal_action(sigint_handler);
	minihell(env, save_stdin, save_stdout);
	return (0);
}

/*
** TODO :
** - Handle errors during command execution.
** - Improve error handling.
*/