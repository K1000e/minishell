/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr				 +#+  +:+      +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 00:00:39 by cgorin			    #+#    #+#            */
/*   Updated: 2024/09/01 02:38:41 by cgorin			   ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

//#include "pipex.h"
#include "minihell.h"

void	exec_pipe(t_pipex *pipex, t_cmd *cmd, t_env *env)
{
	//char	**cmd_args;
	int		i;

	//cmd_args = ft_split(cmd, ' ');
	if (!cmd || !cmd->cmd)
		error(pipex, "command not found: NULL or empty command", 1);
	if (access(cmd->args[0], X_OK) != 0)
		cmd->args[0] = find_executable(cmd->args[0], env);
	if (!cmd->cmd)
		error(pipex, "command not found: no path", 0);
	if (execve(cmd->cmd, cmd->args, env->all) == -1)
	{
		i = -1;
		while (cmd->args[++i])
			free(cmd->args[i]);
		free(cmd->args);
		error(pipex, "command not found", 127);
	}
}

char	*get_path_variable(t_env *env)
{
	t_env	*current;

	current = env->next;
	current = ft_find_key(current, "PATH");
	return(current->value + 1);
	/* while (current != NULL)
	{
		//if (env->key == NULL)
		if (ft_strcmp(current->key, "PATH") == 0)
		{
			return (current->value);
		}
		current = current->next;
	} */
}

char	*find_executable(char *command, t_env *env)
{
	size_t	i;
	char	*path;
	char	*dir;
	char	*full_path;
	int		start;

	printf("Enter in find executable\n");
	path = get_path_variable(env);
	if (!path)
		return (NULL);
	start = 0;
	i = -1;
	while (++i <= ft_strlen(path))
	{
		if (path[i] == ':' || path[i] == '\0')
		{
			dir = ft_substr(path, start, i - start);
			full_path = ft_join(ft_join(dir, "/"), command);
			if (access(full_path, X_OK) == 0)
				return (full_path);
			free(full_path);
			start = i + 1;
		}
	}
	return (command);
}