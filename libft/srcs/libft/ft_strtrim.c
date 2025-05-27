/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 19:13:24 by cgorin            #+#    #+#             */
/*   Updated: 2024/06/18 05:00:22 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static int	start_str(const char *s1, const char *set)
{
	int	x;
	int	i;

	i = 0;
	while (s1[i])
	{
		x = 0;
		while (set[x])
		{
			if (set[x] == s1[i])
				break ;
			x++;
		}
		if (set[x] != s1[i])
			return (i);
		i++;
	}
	return (i);
}

static int	end_str(char const *s1, char const *set)
{
	int	i;
	int	x;

	i = ft_strlen(s1);
	while (i >= 0)
	{
		x = 0;
		while (set[x])
		{
			if (set[x] == s1[i])
				break ;
			x++;
		}
		if (set[x] == s1[i])
			i--;
		else
			return (i);
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	int		i;
	int		start;
	int		end;
	int		len;

	if (!s1)
		return (NULL);
	if (!set)
		return (ft_strdup(s1));
	start = start_str(s1, set);
	end = end_str(s1, set) + 1;
	len = end - start;
	if (len <= 0)
		return (ft_strdup(""));
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (res == NULL)
		return (NULL);
	i = 0;
	while (s1[start] && i < len && start < end)
	{
		res[i++] = s1[start++];
	}
	res[len] = '\0';
	return (res);
}
