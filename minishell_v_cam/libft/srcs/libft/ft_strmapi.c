/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 19:13:21 by cgorin            #+#    #+#             */
/*   Updated: 2024/08/30 14:57:22 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*str;
	int		i;

	if (!s || !f)
		return (NULL);
	str = 0;
	str = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (s[i])
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}

/* s: La chaîne de caractères sur laquelle itérer.
f: La fonction à appliquer à chaque caractère.
Valeur de retour
La chaîne de caractères résultant des applications
successives de ’f’.
Retourne NULL si l’allocation échoue.
Fonctions ex- ternes autorisées
malloc
Description
Applique la fonction ’f’ à chaque caractère de la
chaîne de caractères passée en argument pour créer
une nouvelle chaîne de caractères (avec malloc(3))
résultant des applications successives de ’f’. */