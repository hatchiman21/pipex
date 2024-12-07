/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_processes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:20:35 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/07 14:19:25 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	first_child(int pipefd[2], char *argv[], char **envp)
{
	char	**cmd;
	char	*path;
	int		fd;

	cmd = ft_split(argv[2], ' ');
	path = get_path(cmd[0], envp);
	close(pipefd[0]);
	fd = open(argv[1], O_RDONLY);
	if (!cmd || fd == -1)
	{
		if (cmd)
			free_split(cmd);
		perror(argv[1]);
		exit(EXIT_FAILURE);
	}
	if (dup2(pipefd[1], STDOUT_FILENO) == -1 || dup2(fd, STDIN_FILENO) == -1)
	{
		perror("first_child dub2 failed");
		exit(EXIT_FAILURE);
	}
	close(fd);
	close(pipefd[1]);
	execve(path, cmd, envp);
	perror("first execve failed");
	free_split(cmd);
	free(path);
	exit(EXIT_FAILURE);
}

int	middle_child(char *arg, int tmp_pipefd[2], int pipefd[2], char **envp)
{
	char	**cmd;
	char	*path;

	cmd = ft_split(arg, ' ');
	path = get_path(cmd[0], envp);
	close(pipefd[0]);
	if (!cmd)
		return (close(pipefd[1]), close(tmp_pipefd[0]), -1);
	if (dup2(tmp_pipefd[0], STDIN_FILENO) == -1)
	{
		perror("tmp_pipefd[0] dub2 failed\n");
		exit(EXIT_FAILURE);
	}
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("pipefd[1] dub2 failed\n");
		exit(EXIT_FAILURE);
	}
	close(pipefd[1]);
	close(tmp_pipefd[0]);
	execve(path, cmd, envp);
	perror("middle execve failed");
	free_split(cmd);
	free(path);
	exit(EXIT_FAILURE);
}

int	last_child(int pipefd[2], char *argv[], int argc, char **envp)
{
	char	**cmd;
	char	*path;
	int		fd;

	cmd = ft_split(argv[argc - 2], ' ');
	path = get_path(cmd[0], envp);
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
	{
		perror("last_child dub2 failed");
		exit(127);
	}
	close(pipefd[0]);
	close(fd);
	execve(path, cmd, envp);
	perror("last execve failed");
	free_split(cmd);
	free(path);
	exit(127);
}
