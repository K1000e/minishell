/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:10:50 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/16 02:26:08 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>
# include <stdlib.h>
# include "libft.h"

//Fonctions PRINTF

int		ft_printf(const char *format, ...);
int		ft_print_char(char c);
int		ft_print_p(void *ptr);
int		ft_print_s(char *s);
int		ft_print_i_d(int number);
int		ft_print_x(unsigned int n, char format);
int		ft_print_u(unsigned int number);
int		ft_putnbr_base(unsigned int n, char *base, int base_len);

//Fonctions FPRINTF

int		ft_fprintf(int fd, const char *format, ...);
int		ft_print_char_fd(int fd, char c);
int		ft_print_p_fd(int fd, void *ptr);
int		ft_print_s_fd(int fd, char *s);
int		ft_print_i_d_fd(int fd, int number);
int		ft_print_x_fd(int fd, unsigned int n, char format);
int		ft_print_u_fd(int fd, unsigned int number);
int		ft_putnbr_base_fd(int fd, unsigned int n, char *base, int base_len);

//Fonctions SPRINTF

char	*ft_sprintf(char *buffer, const char *format, ...);
int		ft_print_char_str(char *buffer, char c);
int		ft_print_p_str(char *buffer, void *ptr);
int		ft_print_s_str(char *buffer, char *s);
int		ft_print_i_d_str(char *buffer, int number);
int		ft_print_x_str(char *buffer, unsigned int n, char format);
int		ft_print_u_str(char *buffer, unsigned int number);
int		ft_putnbr_base_str(char *buffer, unsigned int n, char *base,
			int base_len);
#endif