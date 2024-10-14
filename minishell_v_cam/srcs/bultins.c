/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:33:23 by cgorin            #+#    #+#             */
/*   Updated: 2024/10/14 18:52:40 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

void ft_exit(t_cmd *cmd)
{
	int exit_code;
	int nb_args;
	int i;

	if (cmd->args[1] == NULL)
		exit(0);
	nb_args = 0;
	while (cmd->args[nb_args] != NULL)
		nb_args++;
	if (nb_args > 2)
		printf("exit: too many arguments\n");
	else
	{
		i = -1;
		while (cmd->args[1][++i])
		{
			if (!ft_isalnum(cmd->args[1][i]))
			{
				printf("exit: %s: numeric argument required\n", cmd->args[1]);
				exit(2);
			}
		}
		exit_code = ft_atoi(cmd->args[1]) % 256;
		exit(exit_code);
	}
}
