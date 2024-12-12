/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 19:13:19 by cgorin            #+#    #+#             */
/*   Updated: 2024/09/12 12:38:15 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	int		i;
	int		size;

	size = 0;
	if (s1)
		size += ft_strlen(s1);
	if (s2)
		size += ft_strlen(s2);
	res = ft_calloc(sizeof(char), (size + 1));
	if (!res)
		return (NULL);
	i = 0;
	if (s1)
	{
		while (s1[i])
		{
			res[i] = s1[i];
			i++;
		}
	}
	if (s2)
		while (*s2)
			res[i++] = *s2++;
	return (res);
}
