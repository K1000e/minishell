/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 23:52:35 by cgorin            #+#    #+#             */
/*   Updated: 2024/10/15 18:50:57 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

static int	ft_printf_format(const char *format, va_list ap)
{
	int	len;

	len = 0;
	if (*format == 'i' || *format == 'd')
		return (ft_print_i_d(va_arg(ap, int)));
	else if (*format == 'c')
		return (ft_print_char(va_arg(ap, int)));
	else if (*format == 's')
		return (ft_print_s(va_arg(ap, char *)));
	else if (*format == 'x' || *format == 'X')
		return (ft_print_x(va_arg(ap, unsigned int), *format));
	else if (*format == 'p')
		return (ft_print_p(va_arg(ap, void *)));
	else if (*format == '%')
		return (ft_print_char(*format));
	else if (*format == 'u')
		return (ft_print_u(va_arg(ap, unsigned int)));
	return (len);
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		len;

	va_start(ap, format);
	len = 0;
	while (*format)
	{
		if (*format == '%' && ft_strchr("cspdiuxX%", *(format + 1)))
		{
			format++;
			if (ft_strchr("cspdiuxX", *format))
			{
				len += ft_printf_format(format, ap);
				va_arg(ap, void *);
			}
			else if (*format == '%')
				len += (ft_print_char('%'));
		}
		else
			len += (ft_print_char(*format));
		format++;
	}
	va_end(ap);
	return (len);
}

/* int	ft_joinf(const char *format, ...)
{
	va_list	ap;
	int		len;

	va_start(ap, format);
	len = 0;
	while (*format)
	{
		if (*format == '%' && ft_strchr("cspdiuxX%", *(format + 1)))
		{
			format++;
			if (ft_strchr("cspdiuxX", *format))
			{
				len += ft_printf_format(format, ap);
				va_arg(ap, void *);
			}
			else if (*format == '%')
				len += (ft_print_char('%'));
		}
		else
			len += (ft_print_char(*format));
		format++;
	}
	va_end(ap);
	return (len);
} */
