/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 21:45:11 by aatieh            #+#    #+#             */
/*   Updated: 2024/11/12 16:50:04 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	middle_process(int pipefd[2], char *argv[], int argc, char **envp)
{
	int	id;
	int	arg;
	int	tmp_fd;

	arg = 3;
	while (arg + 2 < argc)
	{
		tmp_fd = creat_tmp_fd(pipefd);
		id = fork();
		if (!id)
			return (middle_child(argv[arg], tmp_fd, pipefd, envp));
		else
		{
			if (id == -1)
				ft_putstr_fd("fork failed\n", 2);
			waitpid(id, NULL, 0);
			close(tmp_fd);
			unlink("./src/tempfile");
		}
		arg++;
	}
	return (1);
}

void	last_process(char *argv[], int argc, int pipefd[2], char **envp)
{
	int	id;

	if (middle_process(pipefd, argv, argc, envp) == -1)
		return ;
	id = fork();
	if (!id)
		last_child(pipefd, argv, argc, envp);
	else
	{
		if (id == -1)
			ft_putstr_fd("fork failed\n", 2);
		close(pipefd[0]);
		close(pipefd[1]);
		wait(0);
	}
}

int	first_process(char *argv[], int argc, char **envp)
{
	int	id;
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		return (perror("Pipe failed"), 3);
	if (access(argv[1], F_OK) == -1 || access(argv[1], R_OK) == -1)
	{
		perror(argv[1]);
		last_process(argv, argc, pipefd, envp);
	}
	else
	{
		id = fork();
		if (!id)
			first_child(pipefd, argv, envp);
		else
		{
			if (id == -1)
				ft_putstr_fd("fork failed\n", 2);
			waitpid(id, NULL, 0);
			last_process(argv, argc, pipefd, envp);
		}
	}
	return (0);
}

int	main(int argc, char *argv[], char **envp)
{
	int	fd;

	if (argc < 5)
		return (write(2, "not enough inputs\n", 19), 1);
	if (access(argv[argc - 1], F_OK) == -1)
	{
		fd = open(argv[argc - 1], O_CREAT | O_WRONLY, 0644);
		if (fd != -1)
			close (fd);
	}
	if (!ft_strncmp("here_doc", argv[1], 10))
		limiter_f(argv, argc, envp);
	else
		return (first_process(argv, argc, envp));
	return (0);
}
