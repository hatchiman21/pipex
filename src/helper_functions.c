/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:15:44 by aatieh            #+#    #+#             */
/*   Updated: 2024/11/12 16:53:34 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	dub_failed(int fd1, int fd2)
{
	close(fd1);
	close(fd2);
	ft_putstr_fd("dub2 failed\n", 2);
	return (-1);
}

int	creat_tmp_fd(int pipefd[2])
{
	int		i;
	int		tmp_fd;
	char	buf[1024];

	tmp_fd = open("./src/tempfile",  O_WRONLY | O_CREAT | O_TRUNC, 0666);
	ft_printf("inside pipefd[0] is %d and pipefd[1] is %d and tmp_fd is %d\n", pipefd[0], pipefd[1], tmp_fd);
	if (tmp_fd == -1)
		return (perror("unable to creat tmp file\n"), -1);
	i = read(pipefd[0], buf, 1024);
	while (i == 1024)
	{
		write(tmp_fd, buf, i);
		i = read(pipefd[0], buf, 1024);
	}
	if (i > 0)
		write(tmp_fd, buf, i);
	close(tmp_fd);
	tmp_fd = open("./src/tempfile", O_RDONLY);
	return (tmp_fd);
}

void	free_split(char **string)
{
	int	i;

	i = 0;
	if (!string)
		return ;
	while (string[i])
	{
		free(string[i]);
		i++;
	}
	perror("execve failed\n");
	free(string);
}

char	**get_cmd(char *arg)
{
	char	**cmd;

	cmd = malloc(sizeof(char *) * 4);
	if (!cmd)
		return (ft_putstr_fd("malloc failed\n", 2), NULL);
	cmd[0] = ft_strdup("/bin/sh");
	cmd[1] = ft_strdup("-c");
	cmd[2] = ft_strdup(arg);
	cmd[3] = NULL;
	return (cmd);
}
