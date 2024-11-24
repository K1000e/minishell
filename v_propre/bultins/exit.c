/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 18:33:23 by cgorin            #+#    #+#             */
/*   Updated: 2024/10/19 22:41:44 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

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
				ft_fprintf(2, "exit: %s: numeric argument required\n", cmd->args[1]);
				exit(2);
			}
		}
		exit_code = 0;
		if (nb_args == 2)
			exit_code = ft_atoi(cmd->args[1]) % 256;
		printf("exit\n");
		exit(exit_code);
	}
	ft_fprintf(2, "exit: too many arguments\n");
}