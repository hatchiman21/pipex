/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 21:45:11 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/05 08:51:07 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

char	**get_all_paths(char **envp)
{
	int		i;
	char	**path;

	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5))
		i++;
	path = ft_split(envp[i] + 5, ':');
	return (path);
}

void	print_path(char **path)
{
	int	i;

	i = 0;
	while (path[i])
	{
		ft_printf("path[%d] = %s\n", i, path[i]);
		i++;
	}
}

int	middle_process(int pipefd[2], char *argv[], int argc, char **envp)
{
	int	id;
	int	arg;
	int	tmp_pipefd[2];

	arg = 3;
	while (arg + 2 < argc)
	{
		tmp_pipefd[1] = pipefd[1];
		tmp_pipefd[0] = pipefd[0];
		if (pipe(pipefd) == -1)
			return (perror("Pipe failed"), -1);
		id = fork();
		if (!id)
			return (middle_child(argv[arg], tmp_pipefd, pipefd, envp));
		else
		{
			// close(pipefd[0]);
			close(tmp_pipefd[1]);
			wait(0);
			if (id == -1)
				ft_putstr_fd("fork failed\n", 2);
			waitpid(id, NULL, 0);
			close(tmp_pipefd[0]);
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
		close(pipefd[1]);
		if (id == -1)
			ft_putstr_fd("fork failed\n", 2);
	wait(0);
	waitpid(id, NULL, 0);
		close(pipefd[0]);
	}
}

int	first_process(char *argv[], int argc, char **envp)
{
	int	id;
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		return (perror("Pipe failed"), 3);
	if (access(argv[1], R_OK) == -1)
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
			close(pipefd[1]);
			if (id == -1)
				ft_putstr_fd("fork failed\n", 2);
			wait(0);
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
