/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 19:14:33 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/16 12:44:19 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static int	search_needle(const char *haystack, const char *needle)
{
	size_t	i;

	i = 0;
	while (needle[i])
	{
		if (needle[i] != haystack[i])
			return (0);
		i++;
	}
	return (1);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;

	i = 0;
	if (!needle[i])
		return ((char *)(haystack));
	while (*haystack && i < len)
	{
		if (*haystack == needle[0] && ft_strlen(needle) <= len - i)
			if (search_needle(haystack, needle))
				return ((char *)(haystack));
		haystack++;
		i++;
	}
	return (NULL);
}
