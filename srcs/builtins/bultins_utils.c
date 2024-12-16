/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 02:29:37 by mabdessm          #+#    #+#             */
/*   Updated: 2024/12/16 23:33:48 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

void	ft_cd_env(t_env *env, t_env *current, char *directory)
{
	char	cwd[1024];
	char	*value;

	if (chdir(directory) != 0)
	{
		g_exit_code = 1;
		perror("cd");
	}
	else if (env)
	{
		current = env;
		current = ft_find_key(env, "PWD");
		if (current && getcwd(cwd, sizeof(cwd)) != NULL)
		{
			value = current->value;
			ft_update_key(env, "OLDPWD", value);
			ft_update_key(env, "PWD", cwd);
			g_exit_code = 0;
		}
		else
		{
			g_exit_code = 1;
			perror("getcwd");
		}
	}
}

static void	ft_cd_execution(t_cmd *cmd, t_env *env, char *directory,
		t_env *current)
{
	if (cmd->args[1] == NULL)
	{
		current = ft_find_key(env, "HOME");
		if (current && current->value)
		{
			g_exit_code = 0;
			directory = current->value;
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
	ft_cd_env(env, current, directory);
}

void	ft_cd(t_cmd *cmd, t_env *env)
{
	char	*directory;
	t_env	*current;

	directory = NULL;
	current = NULL;
	ft_cd_execution(cmd, env, directory, current);
}
