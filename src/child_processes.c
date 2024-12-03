/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_processes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:20:35 by aatieh            #+#    #+#             */
/*   Updated: 2024/11/12 17:43:05 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	first_child(int pipefd[2], char *argv[], char **envp)
{
	char	**cmd;
	int		fd;

	cmd = get_cmd(argv[2]);
	close(pipefd[0]);
	fd = open(argv[1], O_RDONLY);
	if (!cmd && fd != -1)
		close(fd);
	if (!cmd || fd == -1)
	{
		if (cmd)
			free_split(cmd);
		return (perror("open failed\n"), close(pipefd[1]), -1);
	}
	if (dup2(fd, STDIN_FILENO) == -1 || dup2(pipefd[1], STDOUT_FILENO) == -1)
		exit(dub_failed(fd, pipefd[1]));
	close(fd);
	close(pipefd[1]);
	execve("/bin/sh", cmd, envp);
	perror("execve failed\n");
	free_split(cmd);
	return (-1);
}

int	middle_child(char *arg, int tmp_fd, int pipefd[2], char **envp)
{
	char	**cmd;

	cmd = get_cmd(arg);
	close(pipefd[0]);
	if (!cmd)
		return (close(pipefd[1]), close(tmp_fd), -1);
	if (dup2(tmp_fd, STDIN_FILENO) == -1)
		ft_putstr_fd("tmp_fd dub2 failed", 2);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		return (ft_putstr_fd("pipefd[1] dub2 failed\n", 2),
			close(pipefd[1]), (-1));
	close(pipefd[1]);
	ft_putstr_fd("exec\n", 2);
	execve("/bin/sh", cmd, envp);
	perror("execve failed\n");
	free_split(cmd);
	return (-1);
}

int	last_child(int pipefd[2], char *argv[], int argc, char **envp)
{
	char	**cmd;
	int		fd;

	cmd = get_cmd(argv[argc - 2]);
	close(pipefd[1]);
	if (!cmd)
		return (close(pipefd[0]), -1);
	if (ft_strncmp(argv[1], "here_doc", 10))
		fd = open(argv[argc - 1], O_WRONLY | O_TRUNC);
	else
		fd = open(argv[argc - 1], O_WRONLY | O_APPEND);
	if (fd == -1)
		return (close(pipefd[1]), perror(argv[argc - 1]), -1);
	if (dup2(pipefd[0], STDIN_FILENO) == -1 || dup2(fd, STDOUT_FILENO) == -1)
		return (dub_failed(fd, pipefd[0]));
	close(pipefd[0]);
	close(fd);
	execve("/bin/sh", cmd, envp);
	perror("execve failed\n");
	free_split(cmd);
	return (-1);
}
