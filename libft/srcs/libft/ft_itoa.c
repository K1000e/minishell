/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 19:12:53 by cgorin            #+#    #+#             */
/*   Updated: 2024/08/30 14:57:22 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static int	nbrlen(int n)
{
	int		len;

	len = 0;
	if (n < 1)
		len++;
	while (n)
	{
		len++;
		n = n / 10;
	}
	return (len);
}

static char	*ft_newstr(int n)
{
	char	*str;

	str = 0;
	str = (char *) malloc(sizeof(char) * (nbrlen(n) + 1));
	if (!str)
		return (NULL);
	return (str);
}

static int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

char	*ft_itoa(int n)
{
	char			*str;
	int				sign;
	int				len;
	unsigned int	nb;

	str = ft_newstr(n);
	len = nbrlen(n);
	nb = ft_abs(n);
	if (!str)
		return (NULL);
	sign = 0;
	if (n < 0)
	{
		sign = 1;
		n = -n;
	}
	str[len] = '\0';
	while (len --)
	{
		str[len] = (nb % 10) + 48;
		nb = (nb / 10);
	}
	if (sign)
		str[0] = '-';
	return (str);
}

/* 
La chaîne de caractères représentant l’entier.
NULL si l’allocation échoue.

Fonctions externes autorisées
malloc

Description
Alloue (avec malloc(3)) et retourne une chaîne
de caractères représentant l’entier ’n’ reçu en
argument. Les nombres négatifs doivent être gérés. 
*/