#include "../include/minihell.h"

t_bool	is_valid_command_format(const char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i] && !isspace(cmd[i]) && cmd[i] != '"' && cmd[i] != '\''
		&& cmd[i] != '|' && cmd[i] != '>' && cmd[i] != '<')
		i++;
	if (cmd[i] && cmd[i] != ' ' && cmd[i] != '|' && cmd[i] != '>'
		&& cmd[i] != '<' && cmd[i] != '\0')
		return (FALSE);
	return (TRUE);
}

void	print_cmd_list(t_cmd *cmd_lst)
{
	t_cmd	*current;
	int		index;
	int		i;

	current = cmd_lst;
	index = 0;
	while (current)
	{
		printf("Command Node %d\n", index);
		if (current->cmd)
			printf("  Command: %s\n", current->cmd);
		if (current->args)
		{
			i = 0;
			while (current->args[i])
			{
				printf("  Arguments[%d]: ", i);
				printf("%s \n", current->args[i]);
				i++;
			}
		}
		printf(" Redirection: %d\n", current->redirection);
		if (current->nb_infile)
		{
			printf("infile -> %d\n", current->nb_infile);
			i = -1;
			while (current->in_file[++i])
				printf("  Input Redirection: %s\n", current->in_file[i]);
		}
		if (current->nb_outfile)
		{
			printf("outfile %d\n", current->nb_outfile);
			i = -1;
			while (current->out_file[++i] && i < current->nb_outfile)
			{
				printf("  Output Redirection: %s\n", current->out_file[i]);
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
	t_cmd	*current;

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

char	*ft_strjoin_free(char *s1, char *s2, int is_free)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	if (s1 && (is_free == 1 || is_free == 3))
		free(s1);
	if (s2 && (is_free >= 2))
		free(s2);
	return (result);
}

static char	*handle_env_variable(const char *input, int *i, t_env *env)
{
	char	*var_name;
	char	*value;
	int		start;
	int		len;

	start = *i;
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	len = *i - start;
	var_name = ft_substr(input, start, len);
	if (!var_name)
		return (NULL);
	value = get_env_var_value(var_name, env);
	free(var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

static char	*handle_special_cases(const char *input, int *i, t_env *env)
{
	if (input[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(g_exit_code));
	}
	else if (input[*i] == '0')
	{
		(*i)++;
		return (ft_strdup(env->executable));
	}
	else if (ft_isdigit(input[*i]))
	{
		(*i)++;
		return (NULL);
	}
	return (NULL);
}

static char	*append_char(char *result, char c)
{
	char	temp[2];
	char	*new_result;

	temp[0] = c;
	temp[1] = '\0';
	new_result = ft_strjoin(result, temp);
	free(result);
	return (new_result);
}

t_bool	is_within_single_quotes(const char *input, int index)
{
	int		i;
	t_bool	single_quote;

	i = 0;
	single_quote = FALSE;
	while (i < index)
	{
		if (input[i] == '\'' && !single_quote)
			single_quote = TRUE;
		else if (input[i] == '\'' && single_quote)
			single_quote = FALSE;
		i++;
	}
	return (single_quote);
}

static char	*process_input(const char *input, t_env *env)
{
	char	*result;
	char	*to_append;
	int		i;

	i = 0;
	result = ft_strdup("");
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] && (input[i + 1] == '?'
				|| ft_isalpha(input[i + 1]) || input[i + 1] == '_'))
		{
			if (is_within_single_quotes(input, i) || input[i + 1] == '"')
			{
				result = append_char(result, input[i++]);
				continue ;
			}
			i++;
			if (ft_isalpha(input[i]) || input[i] == '_')
				to_append = handle_env_variable(input, &i, env);
			else
				to_append = handle_special_cases(input, &i, env);
		}
		else
			to_append = append_char(ft_strdup(""), input[i++]);
		result = ft_strjoin_free(result, to_append, 3);
	}
	return (result);
}

char	*expand_env_vars(char *input, t_env *env)
{
	if (!input)
		return (NULL);
	input = process_input(input, env);
	return (input);
}

void	ft_command(char *line, t_env *env)
{
	char	*expanded_line;
	t_cmd	*commands;
	t_cmd	*tmp;

	//printf("exit code: %d\n", g_exit_code);
	commands = NULL;
	expanded_line = expand_env_vars(line, env);
	if (expanded_line != NULL)
		commands = parse_command(expanded_line);
	(void)env;
	free(expanded_line);
	if (commands == NULL || commands->cmd[0] == '\0')
		return ;
	is_pipe(commands);
	tmp = commands;
	if (tmp->args[0] && (commands->is_pipe || !is_builtin(tmp->args[0])))
		execute_command(tmp, env);
	else if (tmp->args[0] && (!commands->is_pipe && is_builtin(tmp->args[0])))
		single_builtin(tmp, env);
	else if (tmp->args[0] == NULL )
		g_exit_code = redirection_exec_bultins_single(tmp);
	free_cmd_list(commands);
	//printf("exit code: %d\n", g_exit_code);
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

void	minihell(t_env *env, int save_stdin, int save_stdout)
{
	char	*line;

	while (1)
	{
		dup2(save_stdin, STDIN_FILENO);
		dup2(save_stdout, STDOUT_FILENO);
		line = readline("\001\033[1;31m\002🔥 HellShell 🔥 \001\033[0m\002");
		if (line == NULL)
		{
			ft_printf("exit\n");
			break ;
		}
		//g_exit_code = 0;
		if (ft_strlen(line) > 0)
			add_history(line);
		if (!match_quotes(line) || !count_redir(line))
		{
			free(line);
			continue ;
		}
		ft_command(line, env);
		//printf("exit code: %d\n", g_exit_code);
		free(line);
	}
	exit(0);
}

int			g_exit_code = 0;

int	main(int argc, char **argv, char **environment)
{
	t_env	*env;
	int		save_stdin;
	int		save_stdout;

	env = NULL;
	(void)argc;
	(void)argv;
	if (argc == 1)
	{
		save_stdin = dup(STDIN_FILENO);
		save_stdout = dup(STDOUT_FILENO);
		env = get_env(environment, env, argv[0]);
		set_signal_action(sigint_handler);
		minihell(env, save_stdin, save_stdout);
	}
	return (0);
}
