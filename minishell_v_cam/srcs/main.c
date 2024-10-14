#include "minihell.h"

char *get_current_path()
{
	char *path;

	path = getenv("PWD");  // Récupère le chemin à partir de l'environnement
	if (!path)
	{
		perror("getenv");
		return NULL;
	}
	path = ft_strrchr(path, '/'); 
	return strdup(path);  // Retourne une copie du chemin
}

char *get_username()
{
	char *username;

	username = getenv("USER");  // Récupère le utilisateur à partir de l'env
	if (!username)
	{
		perror("getenv");
		return NULL;
	}
	return strdup(username);  // Retourne une copie du nom d'utilisateur
}

char *prompt_hell(int i)
{
	//char *path;
	//char *username;
	char *prompt;
	char	*prompt2;
	char *in;

	/* path = get_current_path();
	if (!path) {
		path = strdup("unknown");
	} */
	//username = get_username();
	/* char *prompt = malloc(1024 * sizeof(char));
	if (!prompt) {
		free(path);
		return NULL;
	} */
	in = ft_itoa(i);
	prompt = ft_strdup("\033[1;31m🔥 HellShell 🔥 \033[0m\033[1;30m(\033");
	prompt2 = ft_strjoin(prompt, "[0m\033[1;31m");
	free(prompt);
	prompt = ft_strjoin(prompt2, in);
	free(in);
	free(prompt2);
	prompt2 = ft_strjoin(prompt, "/100 souls trapped\033[0m\033[1;30m)");
	free(prompt);
	prompt = ft_strjoin(prompt2, " \033[1;31m>>>\033[0m ");
	free(prompt2);
	//free(path);
	return (prompt);
}

t_bool count_redir(const char *line)
{
	int j;

	j = -1;
	while (line[++j])
	{
		if (line[j] == '>')
		{
			if (line[j + 1] == '>')
			{
				if (line[j + 2] == '>')
					return FALSE;
				j++;
			}
			j++;
		}
		if (line[j] == '<')
		{
			if (line[j + 1] == '<')
			{
				if (line[j + 2] == '<')
					return FALSE;
				j++;
			}
			j++;
		}
	}
	return TRUE;
}

t_bool match_quotes(char **env, char *line)
{
	int i;
	t_bool d_q;
	t_bool s_q;

	i = -1;
	d_q = FALSE;
	s_q = FALSE;
	//printf("env : %s\n", env[0]);
	if (!env)
		return (FALSE);
	while (line[++i])
	{
		if (line[i] == '"' && d_q == FALSE && s_q == FALSE)
			d_q = TRUE;
		else if (line[i] == '"' && d_q == TRUE && s_q == FALSE)
		   d_q = FALSE;
		else if (line[i] == '\'' && d_q == FALSE && s_q == FALSE)
			s_q = TRUE;
		else if (line[i] == '\'' && d_q == FALSE && s_q == TRUE)
		   s_q = FALSE;
	}
	if (d_q == TRUE || s_q == TRUE)
	{
		printf("Error: Unmatched quote\n");
		return FALSE;
	}

	return TRUE;
}

void	ft_exec(t_cmd *cmd)
{
	if (ft_strncmp(cmd->cmd, "exit", 4) == 0)
		ft_exit(cmd);
}

int main(int argc, char **argv, char **env)
{
	char *line;
	int i;

	(void)argc;
	(void)argv;
	i = 0;
	while (1)
	{
		char *prompt_hell_e = prompt_hell(i);
		if (i == 100) 
		{
			printf(YELLOW "🔥 Oh no ! The HellShell explodes! 🔥\n" RESET);
			rl_clear_history();
			exit(1);
		}
		line = readline(prompt_hell_e);
		/* if (strncmp(line, "exit", 4) == 0)
		{
			printf(YELLOW "Bye bye!\n" RESET);
			rl_clear_history();
			exit(0);
		} */
		/* if (line)
			execve("../pipex/pipex", &line, env); */
		if (!line)
			continue ;
		add_history(line);

		// test du parsing
		if (!match_quotes(env, line) || !count_redir(line))
		{
			printf("Error\n");
			free(line);
			continue;
		}
		t_cmd *commands = parse_cmd(line);
		t_cmd *tmp = commands;
		int cmd_index = 0;
		while (tmp)
		{
			printf("help\n");
			printf("Commande %d : %s\n", cmd_index, tmp->cmd);
			for (int j = 0; tmp->args && tmp->args[j]; j++)
				printf("  Argument[%d]: %s\n", j, tmp->args[j]);
			ft_exec(tmp);
			tmp = tmp->next;
			cmd_index++;
			printf("end\n");
		}
		printf("help help\n");
		free_cmd_list(commands);
		free (line);
		free(prompt_hell_e);
		i++;
	}
	return (0);
}