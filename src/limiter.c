/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   limiter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:10:05 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/08 18:40:28 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static void	limiter_child_helper(int *tmp_fd, char *argv[])
{
	char	*line;

	if (*tmp_fd == -1)
		return ;
	line = get_next_line(0);
	while (line)
	{
		if (!ft_strncmp(line, argv[2], ft_strlen(argv[2]))
			&& ft_strlen(line) - 1 == ft_strlen(argv[2])
			&& line[ft_strlen(argv[2])] == '\n')
			break ;
		write(*tmp_fd, line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
	}
	if (line)
		free(line);
	close(*tmp_fd);
	*tmp_fd = open("./src/tempfile", O_RDWR);
}

static void	limiter_child(char *argv[], int pipefd[2], int tmp_fd, char **envp)
{
	char	**cmd;
	char	*path;

	path = NULL;
	cmd = ft_split(argv[3], ' ');
	if (cmd && cmd[0])
		path = get_path(cmd[0], envp);
	close(pipefd[0]);
	limiter_child_helper(&tmp_fd, argv);
	if (!cmd || tmp_fd == -1 || dup2(pipefd[1], STDOUT_FILENO) == -1
		|| dup2(tmp_fd, STDIN_FILENO) == -1 || !path)
	{
		free_all(path, cmd);
		close_all(tmp_fd, pipefd[1]);
		perror("here_doc");
		exit(2);
	}
	close_all(tmp_fd, pipefd[1]);
	execve(path, cmd, envp);
	ft_dprintf(2, "pipex : command not found : %s\n", cmd[0]);
	free_all(path, cmd);
	exit(127);
}

void	limiter_process(char *argv[], int argc, t_pipex *vars, char **envp)
{
	int	tmp_fd;

	tmp_fd = open("./src/tempfile", O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (tmp_fd == -1)
		perror("unable to creat tmp file");
	else
	{
		vars->last_id = fork();
		if (!vars->last_id)
			limiter_child(argv, vars->pipefd, tmp_fd, envp);
		else
		{
			waitpid(vars->last_id, NULL, 0);
			close(tmp_fd);
			unlink("./src/tempfile");
		}
	}
	middle_process(argv, argc, vars, envp);
	last_process(argv, argc, vars, envp);
}
