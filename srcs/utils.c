/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 23:31:25 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/16 13:15:51 by codespace        ###   ########.fr       */
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
/* 
void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
} */

void	is_pipe(t_cmd *cmd)
{
	t_cmd	*current;

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
