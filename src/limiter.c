/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   limiter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:10:05 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/07 14:18:17 by aatieh           ###   ########.fr       */
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

static void	limiter_child(int pipefd[2], char *argv[], char **envp)
{
	char	**cmd;
	char	*path;
	int		fd;

	cmd = ft_split(argv[3], ' ');
	path = get_path(cmd[0], envp);
	close(pipefd[0]);
	fd = open("./src/tempfile",  O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (!cmd || limiter_child_helper(fd, argv) == -1)
	{
		if (cmd)
			free_split(cmd);
		exit(127);
	}
	if (dup2(fd, STDIN_FILENO) == -1 || dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		unlink("./src/tempfile");
		perror("limiter dub2 failed\n");
		exit(127);	
	}
	close(fd);
	close(pipefd[1]);
	execve(path, cmd, envp);
	perror("limiter execve failed\n");
	free_split(cmd);
	free(path);
	exit (127);
}

void	limiter_f(char *argv[], int argc, t_pipex *vars, char **envp)
{
	int	id;

	id = fork();
	if (!id)
		limiter_child(vars->pipefd, argv, envp);
	else
		close(vars->pipefd[1]);
	id = fork();
	if (!id)
	{
		if (access(argv[argc - 1], W_OK) == -1)
		{
			perror(argv[argc - 1]);
			exit(0);
		}
	}
	else
	{
		if (id == -1)
			ft_putstr_fd("limiter fork failed\n", 2);
		close(vars->pipefd[1]);
		close(vars->pipefd[0]);
	}
	last_process(argv, argc, vars, envp);
}
