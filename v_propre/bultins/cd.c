/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 18:33:23 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/02 03:08:35 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

void ft_cd(t_cmd *cmd, t_env *env)
{
	char	*directory = NULL;
	t_env	*current;
	char	cwd[1024];

	if (cmd->args[1] == NULL)
	{
		//ft_fprintf(2, "got to home \n");
		current = ft_find_key(env, "HOME");
		if (current && current->value)
		{
			g_exit_code = 0;
			directory = current->value + 1;
		}
		else
		{
			g_exit_code = 1;
			ft_fprintf(2, "cd: HOME not set\n");
			return ;
		}
	}
	else if (cmd->args[1] && cmd->args[2] != NULL)
	{
		g_exit_code = 1;
		ft_fprintf(2, "cd: too many arguments\n");
		return ;
	}
	else 
		directory = cmd->args[1];
	if (chdir(directory) != 0)
	{
		g_exit_code = 1;
		perror("cd");
		return ;
	}
	if (env)
	{
		current = env;
		current = ft_find_key(env, "PWD");
		if (current && getcwd(cwd, sizeof(cwd)) != NULL)
		{
			char *value = current->value;
			ft_update_key(env, "OLDPWD", value);
			ft_update_key(env, "PWD", cwd);
			g_exit_code = 0;
		}
		else
		{
			g_exit_code = 1; 
			perror("getcwd");
			return ;
		}
	}
}