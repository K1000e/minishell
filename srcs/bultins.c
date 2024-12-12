/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:33:23 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/03 01:35:56 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

void	ft_exit(t_cmd *cmd)
{
	int	exit_code;
	int	nb_args;
	int	i;

	nb_args = 0;
	while (cmd->args[nb_args] != NULL)
		nb_args++;
	i = -1;
	if (cmd->args[1] && (cmd->args[1][0] == '-' || cmd->args[1][0] == '+'))
		i++;
	while (cmd->args[1] != NULL && cmd->args[1][++i])
	{
		if (!ft_isdigit(cmd->args[1][i]))
		{
			ft_fprintf(2, "exit: %s: numeric argument required\n",
				cmd->args[1]);
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
	ft_fprintf(2, "exit: too many arguments\n");
	g_exit_code = 1;
}

t_bool	ft_check_option_echo(t_cmd *cmd, int j)
{
	int		i;
	t_bool	option;

	i = 0;
	option = FALSE;
	if (ft_strncmp(cmd->args[j], "-n", 2) == 0)
	{
		option = TRUE;
		while (cmd->args[j][++i])
		{
			if (cmd->args[j][i] != 'n')
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
	if (cmd->args[1])
	{
		option = ft_check_option_echo(cmd, 1);
		i = option;
		while (cmd->args[i + 1] && ft_check_option_echo(cmd, i + 1) == 1)
			i++;
		while (cmd->args[++i])
		{
			printf("%s", cmd->args[i]);
			if (cmd->args[i + 1] != NULL)
				printf(" ");
		}
	}
	if (!option)
		printf("\n");
	g_exit_code = 0;
}

void	ft_pwd(t_env *env)
{
	t_env	*current;
	char	*path;

	current = ft_find_key(env, "PWD");
	path = current->value;
	if (!path)
		return ;
	printf("%s\n", path);
	g_exit_code = 0;
}

void	ft_cd(t_cmd *cmd, t_env *env)
{
	char	*directory;
	t_env	*current;
	char	cwd[1024];
	char	*value;

	directory = NULL;
	if (cmd->args[1] == NULL)
	{
		current = ft_find_key(env, "HOME");
		if (current && current->value)
		{
			g_exit_code = 0;
			directory = current->value ;
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
			value = current->value;
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
