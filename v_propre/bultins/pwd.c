/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 18:33:23 by cgorin            #+#    #+#             */
/*   Updated: 2024/10/19 22:42:33 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

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
