/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sprint_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 11:41:59 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/15 21:40:07 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_putnbr_base_str(char *buffer, unsigned int n, char *base, int base_len)
{
	int	i;

	i = 0;
	if (n >= (unsigned int)base_len)
		i += ft_putnbr_base_str(buffer, n / base_len, base, base_len);
	i += ft_print_char_str(buffer, (base[n % base_len]));
	return (i);
}

static int	ft_print_adress_str(char *buffer, const unsigned long n)
{
	if (n / 16)
		return (ft_print_adress_str(buffer, n / 16)
			+ ft_print_adress_str(buffer, n % 16));
	else if (!(n / 10))
		ft_print_char_str(buffer, n + '0');
	else
		ft_print_char_str(buffer, (char) n - 10 + 'a');
	return (1);
}

int	ft_print_p_str(char *buffer, void *ptr)
{
	unsigned long	n;
	int				i;

	n = (unsigned long)ptr;
	i = 0;
	i += ft_print_s_str(buffer, "0x");
	return (ft_print_adress_str(buffer, (unsigned long) n) + i);
}

int	ft_print_x_str(char *buffer, unsigned int n, char format)
{
	int	i;

	i = 0;
	if (format == 'X')
		i = ft_putnbr_base_str(buffer, n, "0123456789ABCDEF", 16);
	else if (format == 'x')
		i = ft_putnbr_base_str(buffer, n, "0123456789abcdef", 16);
	return (i);
}
