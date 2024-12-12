/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 23:31:25 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/12 16:08:21 by codespace        ###   ########.fr       */
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
