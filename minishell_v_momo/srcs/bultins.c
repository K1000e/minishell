/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:33:23 by cgorin            #+#    #+#             */
/*   Updated: 2024/10/21 12:09:21 by cgorin           ###   ########.fr       */
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
	if (nb_args <= 2)
	{
		i = -1;
		while (cmd->args[1] != NULL && cmd->args[1][++i])
		{
			if (!ft_isalnum(cmd->args[1][i]))
			{
				printf("exit: %s: numeric argument required\n", cmd->args[1]);
				exit(2);
			}
		}
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


void	ft_pwd()
{
	char *path;
	printf("there\n");

	path = getenv("PWD");  // Récupère le chemin à partir de l'environnement
	if (!path)
	{
		perror("getenv");
		return ;
	}
	printf("%s\n", path);
}
