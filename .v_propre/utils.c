/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 03:22:30 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/02 20:24:36 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

char	*ft_strjoin_free(char *s1, char *s2, int is_free)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	if (s1 && (is_free == 1 || is_free == 3))
		free(s1);
	if (s2 && (is_free >= 2))
		free(s2);
	return (result);
}

void	is_pipe(t_cmd *cmd)
{
	t_cmd *current;

	current = cmd;
	while (current)
	{
		if (current->next)
		{
			current->is_pipe = TRUE;
		}
		else
			current->is_pipe = FALSE;
		current = current->next;
	}
}
