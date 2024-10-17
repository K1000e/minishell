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
		ft_export(cmd, env); */
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		ft_unset(cmd, env);
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
	int j; 

	commands = parse_cmd(line);
	if (commands == NULL) // Ensure the parsed commands are valid
        return;
	tmp = commands;
	int cmd_index = 0;
	while (tmp)
	{
		if (tmp->cmd)
			printf("Commande %d : %s\n", cmd_index, tmp->cmd);
		if (tmp->args)
        {
			j = -1;
            while (tmp->args[++j])
                printf("  Argument[%d]: %s\n", j, tmp->args[j]);
        }
		//if (tmp != NULL)
        ft_exec(tmp, env);
		tmp = tmp->next;
		cmd_index++;
	}
	free_cmd_list(commands);
	//free(commands);
	//free(tmp);
}

#include <signal.h>

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
	printf("      ______________________________    . \\  | / .");
	printf("     /                            / \\     \\ \\ / /");
	printf("    |                            | ==========  - -");
	printf("     \\____________________________\\_/     / / \\ \\ ");
	printf("  ______________________________      \\  | / | \\ ");
	printf(" /                            / \\     \\ \\ / /.   .");
	printf("|                            | ==========  - -");
	printf(" \\____________________________\\_/     / / \\ \\    /");
	printf("      ______________________________   / |\\  | /  .");
	printf("     /                            / \\     \\ \\ / /");
	printf("    |                            | ==========  -  - -");
	printf("     \\____________________________\\_/     / / \\ \\ ");
	printf("                                        .  / | \\  .");
	printf(YELLOW "🔥 Oh no ! The HellShell explodes! 🔥\n" RESET);
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
			free(line);
			continue ;
		}
		ft_command(line, env);
		free (line);
		free(prompt_hell_e);
		i++;
	}
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