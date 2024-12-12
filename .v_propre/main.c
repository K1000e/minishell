/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 03:16:55 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/02 03:20:56 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

void	ft_command(char *line, t_env *env)
{
	char	*expanded_line;
	t_cmd	*commands;
	t_cmd	*tmp;

	commands = NULL;
	expanded_line = expand_env_vars(line, env);
	if (expanded_line != NULL)
		commands = parse_command(expanded_line);
	(void) env;
	free(expanded_line);
	if (commands == NULL || commands->cmd[0] == '\0')
		return;
	is_pipe(commands);
	tmp = commands;
	execute_command(tmp, env);
	free_cmd_list(commands);
}

void minihell(t_env *env, int save_stdin, int save_stdout)
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
			break;
		}
		if (ft_strlen(line) > 0)
			add_history(line);
		if (!match_quotes(line) || !count_redir(line))
		{
			free(line);
			continue ;
		}
		ft_command(line, env);
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
