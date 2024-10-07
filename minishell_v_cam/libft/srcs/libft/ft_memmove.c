/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 19:13:54 by cgorin            #+#    #+#             */
/*   Updated: 2024/06/18 05:00:05 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	void	*ptr;

	ptr = dst;
	if ((!src && !dst) || dst == src)
		return (dst);
	if (dst > src)
	{
		while (n--)
			((char *)dst)[n] = ((char *)src)[n];
	}
	else
	{
		while (n--)
			*(char *)dst++ = *(char *)src++;
	}
	return (ptr);
}
