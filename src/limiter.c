/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   limiter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:10:05 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/05 08:41:35 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static int	limiter_child_helper(int tmp_fd, char *argv[])
{
	char	*line;

	if (tmp_fd == -1)
		return (-1);
	line = get_next_line(0);
	while (line && ft_strncmp(line, argv[2], ft_strlen(argv[2])))
	{
		write(tmp_fd, line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
	}
	close(tmp_fd);
	tmp_fd = open("./src/tempfile", O_RDWR);
	return (tmp_fd);
}

static int	limiter_child(int pipefd[2], char *argv[], char **envp)
{
	char	**cmd;
	char	*path;
	int		tmp_fd;

	cmd = ft_split(argv[3], ' ');
	path = get_path(cmd[0], envp);
	close(pipefd[0]);
	tmp_fd = open("./src/tempfile",  O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (!cmd || limiter_child_helper(tmp_fd, argv) == -1)
	{
		if (cmd)
			free_split(cmd);
		return (close(pipefd[1]), -1);
	}
	if (dup2(tmp_fd, STDIN_FILENO) == -1
		|| dup2(pipefd[1], STDOUT_FILENO) == -1)
		return (dub_failed(pipefd[1], tmp_fd));
	close(tmp_fd);
	close(pipefd[1]);
	execve(path, cmd, envp);
	perror("execve failed\n");
	free_split(cmd);
	free(path);
	return (-1);
}

int	limiter_f(char *argv[], int argc, char **envp)
{
	int	id;
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		return (ft_putstr_fd("pipe failed\n", 2), -1);
	id = fork();
	if (!id)
		return (limiter_child(pipefd, argv, envp));
	else
	{
		close(pipefd[1]);
		waitpid(id, NULL, 0);
	}
	id = fork();
	if (!id)
		return (last_child(pipefd, argv, argc, envp));
	else
	{
		if (id == -1)
			ft_putstr_fd("fork failed\n", 2);
		unlink("./src/tempfile");
		close(pipefd[1]);
		wait(0);
		close(pipefd[0]);
	}
	return (0);
}
