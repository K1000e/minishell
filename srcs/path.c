/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:57:23 by mabdessm          #+#    #+#             */
/*   Updated: 2024/12/16 13:02:29 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

char	*get_path_variable(t_env *env)
{
	t_env	*current;

	current = env->next;
	current = ft_find_key(current, "PATH");
	if (current)
		return (current->value);
	else
		return (NULL);
}

t_bool	path_access(t_cmd *command, char *full_path)
{
	if (access(full_path, X_OK) == 0)
	{
		command->args[0] = ft_strdup(full_path);
		command->path = TRUE;
		free(full_path);
		return (TRUE);
	}
	return (FALSE);
}

void	find_executable(t_cmd *command, t_env *env)
{
	size_t	i;
	char	*path;
	char	*dir;
	char	*full_path;
	int		start;

	command->path = FALSE;
	path = get_path_variable(env);
	if (!path)
		return ;
	start = 0;
	i = -1;
	while (++i <= ft_strlen(path))
	{
		if (path[i] == ':' || path[i] == '\0')
		{
			dir = ft_substr(path, start, i - start);
			full_path = ft_join(ft_join(dir, "/"), command->args[0]);
			if (path_access(command, full_path))
				return ;
			free(full_path);
			start = i + 1;
		}
	}
}
