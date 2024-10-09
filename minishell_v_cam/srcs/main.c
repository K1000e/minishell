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
	char *path;
	char *username;
	char *prompt;
	char *in;

	path = get_current_path();
	if (!path) {
		path = strdup("unknown");
	}
	username = get_username();
	/* char *prompt = malloc(1024 * sizeof(char));
	if (!prompt) {
		free(path);
		return NULL;
	} */
	in = ft_itoa(i);
	prompt = ft_strdup("\033[1;31m🔥 HellShell 🔥 \033[0m\033[1;30m(\033");
	prompt = ft_strjoin(prompt, "[0m\033[1;31m");
	prompt = ft_strjoin(prompt, in);
	free(in);
	prompt = ft_strjoin(prompt, "/100 souls trapped\033[0m\033[1;30m)");
	prompt = ft_strjoin(prompt, " \033[1;31m>>>\033[0m ");
	free(path);
	return prompt;
}


t_bool check_error(char **env, char *line)
{
	int i;
	t_bool d_q;
	t_bool s_q;

	i = -1;
	d_q = FALSE;
	s_q = FALSE;
	//printf("env : %s\n", env[0]);
	if (!env)
		return FALSE;
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


int main(int argc, char **argv, char **env)
{
	char *line;
	int i;

	(void)argc;
	(void)argv;
	i = 0;
	while (1)
	{
		if (i == 100) 
		{
			printf(YELLOW "🔥 Oh no ! The HellShell explodes! 🔥\n" RESET);
			rl_clear_history();
			exit(1);
		}
		line = readline(prompt_hell(i));
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
		if (check_error(env, line) == FALSE)
		{
			printf("Error\n");
			free(line);
			continue;
		}
		t_list *commands = parse_cmd(line);
		t_list *tmp = commands;
		int cmd_index = 0;
		while (tmp)
		{
			printf("Commande %d :\n", cmd_index);
			char **cmd_tokens = (char **)tmp->content;
			for (int i = 0; cmd_tokens[i] != NULL; i++)
				printf("  Token %d: %s\n", i, cmd_tokens[i]);
			tmp = tmp->next;
			cmd_index++;
		}
		tmp = commands;
		while (tmp)
		{
			char **cmd_tokens = (char **)tmp->content;
			for (int i = 0; cmd_tokens[i] != NULL; i++) 
				free(cmd_tokens[i]);
			free(cmd_tokens);
			t_list *next = tmp->next;
			free(tmp);
			tmp = next;
		}
		free (line);
		i++;
	}
	printf("sorted\n");
	return 0 ;
}
