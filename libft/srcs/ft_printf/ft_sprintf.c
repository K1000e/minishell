/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 23:52:35 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/15 07:40:31 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

static void	ft_fprintf_format(char *buffer, int *len, const char *format,
	va_list ap)
{
	int	size;

	size = 0;
	if (*format == 'i' || *format == 'd')
		return (ft_print_i_d_str(&buffer[*len], va_arg(ap, int)));
	else if (*format == 'c')
		return (ft_print_char_str(&buffer[*len], va_arg(ap, int)));
	else if (*format == 's')
		return (ft_print_s_str(&buffer[*len], va_arg(ap, char *)));
	else if (*format == 'x' || *format == 'X')
		return (ft_print_x_str(&buffer[*len], va_arg(ap, unsigned int), *format));
	else if (*format == 'p')
		return (ft_print_p_str(&buffer[*len], va_arg(ap, void *)));
	else if (*format == '%')
		return (ft_print_char_str(&buffer[*len],*format));
	else if (*format == 'u')
		return (ft_print_u_str(&buffer[*len], va_arg(ap, unsigned int)));
	*len += size;
}

int	ft_fprintf(char *buffer, const char *format, ...)
{
	va_list	ap;
	int		len;

	va_start(ap, format);
	len = 0;
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (ft_strchr("cspdiuxX", *format))
				len = ft_fprintf_format(buffer, format, ap);
			else if (*format == '%')
				buffer[len++] = '%';
		}
		else
			buffer[len++] = *format;
		format++;
	}
	va_end(ap);
	return (len);
}
