/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_processes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:20:35 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/07 21:32:56 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	first_child(int pipefd[2], char *argv[], char **envp)
{
	char	**cmd;
	char	*path;
	int		fd;

	cmd = NULL;
	cmd = ft_split(argv[2], ' ');
	path = get_path(cmd[0], envp);
	close(pipefd[0]);
	fd = open(argv[1], O_RDONLY);
	if (!cmd || fd == -1 || dup2(pipefd[1], STDOUT_FILENO) == -1
		|| dup2(fd, STDIN_FILENO) == -1)
	{
		if (cmd)
			free(cmd);
		free(path);
		close_all(fd, pipefd[1]);
		perror(argv[1]);
		exit(2);
	}
	close_all(fd, pipefd[1]);
	execve(path, cmd, envp);
	ft_dprintf(2, "pipex : command not found : %s\n", cmd[0]);
	free_all(path, cmd);
	exit(127);
}

int	middle_child(char *arg, int tmp_pipefd[2], int pipefd[2], char **envp)
{
	char	**cmd;
	char	*path;

	cmd = NULL;
	cmd = ft_split(arg, ' ');
	path = get_path(cmd[0], envp);
	close(pipefd[0]);
	if (!cmd || dup2(tmp_pipefd[0], STDIN_FILENO) == -1
		|| dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("middle dub2 failed\n");
		if (cmd)
			free(cmd);
		free(path);
		close_all(tmp_pipefd[0], pipefd[1]);
		exit(2);
	}
	close_all(tmp_pipefd[0], pipefd[1]);
	execve(path, cmd, envp);
	ft_dprintf(2, "pipex : command not found : %s\n", cmd[0]);
	free_all(path, cmd);
	exit(127);
}

int	last_child(int pipefd[2], char *argv[], int argc, char **envp)
{
	char	**cmd;
	char	*path;
	int		fd;

	cmd = NULL;
	cmd = ft_split(argv[argc - 2], ' ');
	path = get_path(cmd[0], envp);
	close(pipefd[1]);
	if (ft_strncmp(argv[1], "here_doc", 10) != 0)
		fd = open(argv[argc - 1], O_WRONLY | O_TRUNC);
	else
		fd = open(argv[argc - 1], O_WRONLY | O_APPEND);
	if (!cmd || fd == -1 || dup2(pipefd[0], STDIN_FILENO) == -1
		|| dup2(fd, STDOUT_FILENO) == -1 || !path)
	{
		close_all(fd, pipefd[0]);
		free_all(path, cmd);
		perror("last_child dub2 failed");
		exit(2);
	}
	close_all(fd, pipefd[0]);
	execve(path, cmd, envp);
	ft_dprintf(2, "pipex : command not found : %s\n", cmd[0]);
	free_all(path, cmd);
	exit(127);
}
