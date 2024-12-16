/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 23:31:25 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/16 13:07:44 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

/* void	is_pipe(t_cmd *cmd)
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
} */

int	ascii_art(char *line) // a deplacer 
{
	if (ft_strcmp(line, "kitty") == 0 || ft_strcmp(line, "KITTY") == 0
		|| ft_strcmp(line, "Kitty") == 0)
	{
		ft_kitty();
		return (1);
	}
	if (ft_strcmp(line, "chill") == 0 || ft_strcmp(line, "chillguy") == 0
		|| ft_strcmp(line, "CHILL") == 0 || ft_strcmp(line, "CHILLGUY") == 0
		|| ft_strcmp(line, "Chillguy") == 0 || ft_strcmp(line, "Chill") == 0)
	{
		print_chill_guy();
		return (1);
	}
	return (0);
}

void	ft_command(char *line, t_env *env)
{
	char	*expanded_line;
	t_cmd	*commands;
	t_cmd	*tmp;

	if (ascii_art(line))
		return ;
	commands = NULL;
	expanded_line = expand_env_vars(line, env);
	if (expanded_line != NULL)
		commands = parse_command(expanded_line);
	free(expanded_line);
	if (commands == NULL || commands->cmd[0] == '\0')
		return ;
	is_pipe(commands);
	tmp = commands;
	g_exit_code = 0;
	if (tmp->args[0] && (commands->is_pipe || !is_builtin(tmp->args[0])))
		execute_command(tmp, env);
	else if (tmp->args[0] && (!commands->is_pipe && is_builtin(tmp->args[0])))
		execute_builtin(tmp, env, TRUE);
	else if (tmp->args[0] == NULL)
		if (tmp->redirection)
			execute_builtin(tmp, env, TRUE);
	free_cmd_list(commands);
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
		env = create_env(environment, env, argv[0], -1);
		set_signal_action(sigint_handler);
		minihell(env, save_stdin, save_stdout);
	}
	return (0);
}
