/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_s.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 22:46:44 by cgorin            #+#    #+#             */
/*   Updated: 2024/08/30 14:57:22 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_print_s(char *s)
{
	if (s == NULL)
	{
		ft_print_s("(null)");
		return (6);
	}
	write(1, s, ft_strlen(s));
	return (ft_strlen(s));
}
