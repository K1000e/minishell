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

#include "pipex.h"

void	exec_pipe(t_pipex *pipex, char *cmd, char **env)
{
	char	**cmd_args;
	int		i;

	cmd_args = ft_split(cmd, ' ');
	if (!cmd || !*cmd)
		error(pipex, "command not found: NULL or empty command", 1);
	if (access(cmd, X_OK) != 0)
		cmd_args[0] = find_executable(cmd_args[0], env);
	if (!cmd_args[0])
		error(pipex, "command not found: no path", 0);
	if (execve(cmd_args[0], cmd_args, env) == -1)
	{
		i = -1;
		while (cmd_args[++i])
			free(cmd_args[i]);
		free(cmd_args);
		error(pipex, "command not found", 127);
	}
}

char	*get_path_variable(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

char	*find_executable(char *command, char **env)
{
	size_t	i;
	char	*path;
	char	*dir;
	char	*full_path;
	int		start;

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
