/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:33:23 by cgorin            #+#    #+#             */
/*   Updated: 2024/10/24 14:17:17 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"
/*
echo X
cd
pwd X
export X
unset X
env X
exit X
*/
void	ft_exit(t_cmd *cmd)
{
	int exit_code;
	int nb_args;
	int i;

	nb_args = 0;
	while (cmd->args[nb_args] != NULL)
		nb_args++;
	i = -1;
	while (cmd->args[1] != NULL && cmd->args[1][++i])
	{
		if (!ft_isdigit(cmd->args[1][i]))
		{
			printf("exit: %s: numeric argument required\n", cmd->args[1]);
			exit(2);
		}
	}
	if (nb_args <= 2)
	{
		exit_code = 0;
		if (nb_args == 2)
			exit_code = ft_atoi(cmd->args[1]) % 256;
		printf("exit\n");
		exit(exit_code);
	}
	printf("exit: too many arguments\n");
}

t_bool ft_check_option_echo(t_cmd *cmd)
{
	int		i;
	t_bool	option;
	
	i = 0;
	option = FALSE;
	if (ft_strncmp(cmd->args[1], "-n", 2) == 0)
	{
		option = TRUE;
		while (cmd->args[1][++i])
		{
			if (cmd->args[1][i] != 'n')
				option = FALSE;
		}
	}
	return (option);
}

void	ft_echo(t_cmd *cmd)
{
	int		i;
	t_bool	option;

	option = FALSE;
	i = 0;
	if (cmd->args[1])
	{
		option = ft_check_option_echo(cmd);
		i = option;
		while (cmd->args[++i])
		{
			printf("%s",cmd->args[i]);
			if (cmd->args[i + 1] != NULL)
				printf(" ");
		}
	}
	if (!option)
		printf("\n");
}


void	ft_pwd(t_env *env)
{
	t_env	*current;
	char	*path;
	
	printf("there\n");

	current = ft_find_key(env, "PWD");
	path = current->value + 1;
	if (!path)
	{
		perror("getenv");
		return ;
	}
	printf("%s\n", path);
}

void ft_cd(t_cmd *cmd, t_env *env)
{
	char	*directory = NULL;
	t_env	*current;
	char	cwd[1024];

	if (cmd->args[1] == NULL)
	{
		printf("got to home \n");
		current = ft_find_key(env, "HOME");
		if (current && current->value)
		{
			directory = current->value + 1;
		printf("directory =%s!\n", directory);
		}
		else
		{
			printf("cd: HOME not set\n");
			return ;
		}
	}
	else if (cmd->args[1] && cmd->args[2] != NULL)
	{
		printf("cd: too many arguments\n");
		return ;
	}
	else 
		directory = cmd->args[1];
	if (chdir(directory) != 0)
	{
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
		}
		else
		{
			perror("getcwd");
			return ;
		}
	}
}
