#include "minihell.h"

void	ft_exec(t_cmd *cmd, t_env *env)
{
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		ft_exit(cmd);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		ft_echo(cmd);
	/* else if (ft_strcmp(cmd->args[0], "cd") == 0)
		ft_cd(cmd, env); */
	/* else if (ft_strcmp(cmd->args[0], "export") == 0)
		ft_cd(cmd, env); */
	/* else if (ft_strcmp(cmd->args[0], "unset") == 0)
		ft_cd(cmd, env); */
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		ft_env(cmd, env);
	/* else 
		execve("../pipex/pipex", &cmd, env); */
}

void	ft_command(char *line, t_env *env)
{
	t_cmd	*commands;
	t_cmd	*tmp;

	commands = parse_cmd(line);
	tmp = commands;
	int cmd_index = 0;
	while (tmp)
	{
	// test du parsing
		//printf("Commande %d : %s\n", cmd_index, tmp->cmd);
		//for (int j = 0; tmp->args && tmp->args[j]; j++)
			//printf("  Argument[%d]: %s\n", j, tmp->args[j]);
		
		ft_exec(tmp, env);
		tmp = tmp->next;
		cmd_index++;
	}
	free_cmd_list(commands);
	free(commands);
	free(tmp);
}


int main(int argc, char **argv, char **environment)
{
	char	*line;
	int		i;
	t_env	*env = NULL;

	char	*prompt_hell_e;

	(void)argc;
	(void)argv;
	i = 0;
	env = get_env(environment, env);
	while (1)
	{
		prompt_hell_e = prompt_hell(i);
		if (i == 100) 
		{
			printf(YELLOW "🔥 Oh no ! The HellShell explodes! 🔥\n" RESET);
			rl_clear_history();
			exit(1);
		}
		line = readline(prompt_hell_e);
		if (!line)
			continue ;
		add_history(line);
		if (!match_quotes(line) || !count_redir(line))
		{
			printf("Error\n");
			free(line);
			continue;
		}
		ft_command(line, env);
	/* 	commands = parse_cmd(line);
		tmp = commands;
		int cmd_index = 0;
		while (tmp)
		{
		// test du parsing
			//printf("Commande %d : %s\n", cmd_index, tmp->cmd);
			//for (int j = 0; tmp->args && tmp->args[j]; j++)
				//printf("  Argument[%d]: %s\n", j, tmp->args[j]);
			ft_exec(tmp, env);
			tmp = tmp->next;
			cmd_index++;
		} */
		/* free_cmd_list(commands);
		free(commands);
		free(tmp); */
		free (line);
		free(prompt_hell_e);
		i++;
	}
	return (0);
}