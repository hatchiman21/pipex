/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 18:14:06 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/25 06:33:48 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include "../ft_printf.h"

int	print_string(char *string, t_printf *res, int *i)
{
	int	j;

	j = *i;
	while (string[*i + 1] && string[*i + 1] != '%')
		*i += 1;
	res->str = malloc(sizeof(char) * *i - j + 2);
	if (!res->str)
		return (0);
	res->len = write_printf((string + j), res->str, *i - j + 1);
	return (res->len);
}

int	ft_printf_helper_step(t_printf **res, t_printf *head, char *string, int i)
{
	if (!(*res)->str)
	{
		free_printf(head);
		write(2, "Malloc failed in ft_printf\n", 28);
		return (-1);
	}
	if (string[i])
	{
		(*res)->next = malloc(sizeof(t_printf));
		if (!(*res)->next)
		{
			free_printf(head);
			write(2, "Malloc failed in ft_printf\n", 28);
			return (-1);
		}
		(*res) = (*res)->next;
	}
	(*res)->next = NULL;
	return (0);
}

t_printf	*ft_printf_helper(char *string, va_list *args, int *count)
{
	t_printf	*res;
	t_printf	*head;
	int			i;

	i = 0;
	res = NULL;
	head = NULL;
	res = malloc(sizeof(t_printf));
	if (!res)
	{
		write(2, "Malloc failed in ft_printf\n", 28);
		return (NULL);
	}
	head = res;
	while (string[i])
	{
		if (string[i] == '%')
			*count += print_main(string, *args, 1 + i++, res);
		else
			*count += print_string(string, res, &i);
		i++;
		if (ft_printf_helper_step(&res, head, string, i) == -1)
			return (NULL);
	}
	return (head);
}

int	ft_printf(const char *string, ...)
{
	int			count;
	char		*final_res;
	va_list		args;
	t_printf	*res;

	if (!string)
		return (0);
	count = 0;
	va_start (args, string);
	res = ft_printf_helper((char *)string, &args, &count);
	va_end(args);
	if (!res)
		return (0);
	final_res = ft_strjoin_printf(res, count);
	free_printf(res);
	if (!final_res)
	{
		write(2, "Malloc failed in ft_printf final step\n", 39);
		return (0);
	}
	write(1, final_res, ft_strlen(final_res));
	free(final_res);
	return (count);
}

int	ft_dprintf(int fd, const char *string, ...)
{
	int			count;
	char		*final_res;
	va_list		args;
	t_printf	*res;

	if (!string)
		return (0);
	count = 0;
	va_start (args, string);
	res = ft_printf_helper((char *)string, &args, &count);
	va_end(args);
	if (!res)
		return (0);
	final_res = ft_strjoin_printf(res, count);
	free_printf(res);
	if (!final_res)
	{
		write(2, "Malloc failed in ft_printf final step\n", 39);
		return (0);
	}
	write(fd, final_res, ft_strlen(final_res));
	free(final_res);
	return (count);
}
