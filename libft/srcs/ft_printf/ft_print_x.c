/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_x.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 11:47:32 by cgorin            #+#    #+#             */
/*   Updated: 2024/08/30 14:57:22 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_print_x(unsigned int n, char format)
{
	int	i;

	i = 0;
	if (format == 'X')
		i = ft_putnbr_base(n, "0123456789ABCDEF", 16);
	else if (format == 'x')
		i = ft_putnbr_base(n, "0123456789abcdef", 16);
	return (i);
}
