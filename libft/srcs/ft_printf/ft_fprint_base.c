/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 11:41:59 by cgorin            #+#    #+#             */
/*   Updated: 2024/08/30 14:57:22 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"


int	ft_putnbr_base_fd(int fd, unsigned int n, char *base, int base_len)
{
	int	i;

	i = 0;
	if (n >= (unsigned int)base_len)
		i += ft_putnbr_base_fd(fd, n / base_len, base, base_len);
	i += ft_print_char_fd(fd, (base[n % base_len]));
	return (i);
}


static int	ft_print_adress_fd(int fd, const unsigned long n)
{
	if (n / 16)
		return (ft_print_adress_fd(fd, n / 16) + ft_print_adress_fd(fd, n % 16));
	else if (!(n / 10))
		ft_print_char_fd(fd, n + '0');
	else
		ft_print_char_fd(fd, (char) n - 10 + 'a');
	return (1);
}

int	ft_print_p_fd(int fd, void *ptr)
{
	unsigned long	n;
	int				i;

	n = (unsigned long)ptr;
	i = 0;
	i += ft_print_s_fd(fd, "0x");
	return (ft_print_adress_fd(fd, (unsigned long) n) + i);
}

int	ft_print_x_fd(int fd, unsigned int n, char format)
{
	int	i;

	i = 0;
	if (format == 'X')
		i = ft_putnbr_base_fd(fd, n, "0123456789ABCDEF", 16);
	else if (format == 'x')
		i = ft_putnbr_base_fd(fd, n, "0123456789abcdef", 16);
	return (i);
}