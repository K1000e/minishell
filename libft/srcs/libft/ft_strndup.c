/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 19:14:30 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/16 12:44:35 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strndup(char *str, size_t len)
{
	char	*dup;
	size_t	len_dup;

	len_dup = ft_strlen(str);
	if (len_dup > len)
		len_dup = len;
	dup = ft_calloc((len_dup + 1), sizeof(char));
	if (!dup)
		return (NULL);
	ft_strlcpy(dup, str, len_dup + 1);
	return (dup);
}
