/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sprint_num.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 22:47:19 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/15 21:45:13 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

static int	nbrlen(int n)
{
	int	len;

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

int	ft_print_i_d_str(char *buffer, int number)
{
	int	i;

	i = nbrlen(number);
	ft_putnbr_base_str(buffer, number, "0123456789", 10);
	return (i);
}

int	ft_print_u_str(char *buffer, unsigned int number)
{
	int		i;
	char	*base;

	i = 0;
	base = "0123456789";
	if (number >= 10)
		i += ft_print_u_str(buffer, number / 10);
	i += ft_print_char_str(buffer, *(base + (number % 10)));
	return (i);
}
