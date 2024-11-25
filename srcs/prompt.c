#include "../include/minihell.h"

/* 
** Fonction : get_current_path
** Retourne le chemin actuel en récupérant la variable d'environnement "PWD".
*/

char	*get_current_path()
{
	char *path;

	path = getenv("PWD");
	if (!path)
	{
		perror("getenv");
		return NULL;
	}
	path = ft_strrchr(path, '/'); 
	return strdup(path);
}

/* 
** Fonction : get_username
** Retourne le nom d'utilisateur en récupérant la variable d'environnement "USER".
*/

char	*get_username()
{
	char *username;

	username = getenv("USER");
	if (!username)
	{
		perror("getenv");
		return NULL;
	}
	return strdup(username);
}

/* 
** Fonction : prompt_hell
** Crée l'invite personnalisée pour le shell avec un compteur d'âmes piégées.
*/

char	*prompt_hell(int i)
{
	//char *path;
	//char *username;
	char	*prompt;
	char	*prompt2;
	char	*in;

	/* path = get_current_path();
	if (!path) {
		path = strdup("unknown");
	} */
	//username = get_username();
	in = ft_itoa(i);
	prompt = ft_strjoin("\001\033[1;31m\002🔥 HellShell 🔥 ", in);
	free(in);
	if (!prompt)
		return NULL;
	prompt2 = ft_strjoin(prompt, "/100 souls trapped >>>\001\033[0m\002 ");
	free(prompt);
	if (!prompt2)
		return NULL;
	return (prompt2);
}
