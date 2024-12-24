/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 18:14:06 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/24 17:37:14 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int write_printf(char *src, char *dest, int char_num)
{
	int	i;

	i = 0;
	while (src[i] && i < char_num)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;
	return (i);
}

int write_char_printf(char c, char *dest)
{
	dest[0] = c;
	dest[1] = '\0';
	return (1);
}

static char	*empty_check(char *string)
{
	if (!string)
		return ("(null)");
	return (string);
}

int	nbr_alloc(long long n, t_printf *res, int j, int base)
{
	int	i;

	i = 0;
	if (j == 3)
		i += 2;
	if (j == 3 && n == 0)
		i = 5;
	else if (n == 0)
		i++;
	if (n < 0)
	{
		i++;
		n *= -1;
	}
	while (n > 0)
	{
		i++;
		n /= base;
	}
	res->str = malloc(sizeof(char) * (i + 1));
	return (i);
}

int	ft_putnbr_hex_print(unsigned long n, t_printf *res, int j, int *i)
{
	if (j == 3 && n != 0)
		*i += write_printf("0x", res->str, 2);
	else if (j == 3 && n == 0)
		return (write_printf("(nil)", res->str, 5));
	if (n > 15)
	{
		if (j == 2)
			ft_putnbr_hex_print(n / 16, res , 2, i);
		else
			ft_putnbr_hex_print(n / 16, res, 1, i);
	}
	if ((n % 16) < 10)
		*i += write_char_printf((n % 16) + '0', res->str + *i);
	else if (j == 2)
		*i += write_char_printf((n % 16) - 10 + 'A', res->str + *i);
	else
		*i += write_char_printf((n % 16) - 10 + 'a', res->str + *i);
	return (*i + 1);
}

int	ft_putnbr_print(long n, t_printf *res, int *i)
{
	long	m;
	char	r;

	m = n;
	if (m < 0)
	{
		*i += write_char_printf('-', res->str);
		m *= -1;
	}
	if (m > 9)
		ft_putnbr_print(m / 10, res, i);
	r = (m % 10) + '0';
	*i += write_char_printf(r, res->str + *i);
	return (*i + 1);
}

static int	print_main(char *string, va_list args, int i, t_printf *res)
{
	int				count;
	char			c;
	unsigned long	nbr;
	int				n;
	int 			m;

	count = 0;
	m = 0;
	if (string[i] == 'c')
	{
		count++;
		c = va_arg(args, int);
		res->str = ft_strdup(&c);
	}
	else if (string[i] == 's')
	{
		res->str = ft_strdup(empty_check(va_arg(args, char *)));
		count = ft_strlen(res->str);
	}
	else if (string[i] == 'p')
	{
		nbr = (size_t)va_arg(args, void *);
		res->str = malloc(sizeof(char) * 19);
		ft_putnbr_hex_print(nbr, res, 3, &m);
		count = ft_strlen(res->str);
	}
	else if (string[i] == 'd' || string[i] == 'i')
	{
		n = va_arg(args, int);
		count = nbr_alloc(n, res, 0, 10);
		ft_putnbr_print(n, res, &m);
	}
	else if (string[i] == 'u')
	{
		nbr = va_arg(args, unsigned int);
		count = nbr_alloc(nbr, res, 0, 10);
		ft_putnbr_print(nbr, res, &m);
	}
	else if (string[i] == 'x')
	{
		nbr = va_arg(args, unsigned int);
		count = nbr_alloc(nbr, res, 1, 16);
		ft_putnbr_hex_print(nbr, res, 1, &m);
	}
	else if (string[i] == 'X')
	{
		nbr = va_arg(args, unsigned int);
		count = nbr_alloc(nbr, res, 2, 16);
		ft_putnbr_hex_print(nbr, res, 2, &m);
	}
	else if (string[i] == '%')
	{
		count++;
		res->str = ft_strdup("%");
	}
	return (count);
}

char	*ft_strjoin_printf(t_printf *og_res)
{
	t_printf	*tmp;
	char    	*res;
	int	    	i;
	int	    	j;

	i = 0;
	tmp = og_res;
	while (tmp)
	{
		i += ft_strlen(tmp->str);
		tmp = tmp->next;
	}	
	res = malloc(sizeof(char) * (i + 1));
	i = 0;
	if (!res)
		return (NULL);
	tmp = og_res;
	while (tmp)
	{
		j = 0;
		while (tmp->str[j])
			res[i++] = tmp->str[j++];
		tmp = tmp->next;
	}
	res[i] = '\0';
	return (res);
}

void	free_printf(t_printf *res)
{
	t_printf	*tmp;

	while (res)
	{
		tmp = res->next;
		free(res->str);
		free(res);
		res = tmp;
	}
}

int	ft_printf(const char *string, ...)
{
	int			count;
	int			i;
	int			j;
	char		*final_res;
	va_list		args;
	t_printf	*head;
	t_printf	*res;

	if (!string)
		return (0);
	i = 0;
	count = 0;
	res = NULL;
	head = NULL;
	va_start (args, string);
	while (string[i])
	{
		if (!res)
			res = malloc(sizeof(t_printf));
		else
		{
			res->next = malloc(sizeof(t_printf));
			res = res->next;
		}
		if (!head)
			head = res;
		res->next = NULL;
		if (string[i] == '%')
			count += print_main((char *)string, args, 1 + i++, res);
		else
		{
			j = i;
			while (string[i + 1] && string[i + 1] != '%')
				i++;
			res->str = malloc(sizeof(char) * i - j + 2);
			count += write_printf((char *)(string + j), res->str, i - j + 1);
		}
		i++;
	}
	final_res = ft_strjoin_printf(head);
	free_printf(head);
	va_end(args);
	write(1, final_res, ft_strlen(final_res));
	free(final_res);
	return (count);
}

int	ft_dprintf(int fd, const char *string, ...)
{
	int			count;
	int			i;
	int			j;
	char		*final_res;
	va_list		args;
	t_printf	*head;
	t_printf	*res;

	if (!string)
		return (0);
	i = 0;
	count = 0;
	res = NULL;
	head = NULL;
	va_start (args, string);
	while (string[i])
	{
		if (!res)
			res = malloc(sizeof(t_printf));
		else
		{
			res->next = malloc(sizeof(t_printf));
			res = res->next;
		}
		if (!head)
			head = res;
		res->next = NULL;
		if (string[i] == '%')
			count += print_main((char *)string, args, 1 + i++, res);
		else
		{
			j = i;
			while (string[i + 1] && string[i + 1] != '%')
				i++;
			res->str = malloc(sizeof(char) * i - j + 2);
			count += write_printf((char *)(string + j), res->str, i - j + 1);
		}
		i++;
	}
	final_res = ft_strjoin_printf(head);
	free_printf(head);
	va_end(args);
	write(fd, final_res, ft_strlen(final_res));
	free(final_res);
	return (count);
}
