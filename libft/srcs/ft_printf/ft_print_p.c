/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_p.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 19:12:53 by cgorin            #+#    #+#             */
/*   Updated: 2024/08/30 14:57:22 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

static int	ft_print_adress(const unsigned long n)
{
	if (n / 16)
		return (ft_print_adress(n / 16) + ft_print_adress(n % 16));
	else if (!(n / 10))
		ft_print_char(n + '0');
	else
		ft_print_char((char) n - 10 + 'a');
	return (1);
}

int	ft_print_p(void *ptr)
{
	unsigned long	n;
	int				i;

	n = (unsigned long)ptr;
	i = 0;
	i += ft_print_s("0x");
	return (ft_print_adress((unsigned long) n) + i);
}
