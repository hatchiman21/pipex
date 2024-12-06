/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 21:45:11 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/05 20:22:21 by aatieh           ###   ########.fr       */
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

// void	print_path(char **path)
// {
// 	int	i;

// 	i = 0;
// 	while (path[i])
// 	{
// 		ft_printf("path[%d] = %s\n", i, path[i]);
// 		i++;
// 	}
// }

void	wait_for_all(t_pipex *vars)
{
	while (vars->children_num > 0)
	{
		waitpid(-1, NULL, 0);
		vars->children_num -= 1;
	}
}

void	last_process(char *argv[], int argc, t_pipex *vars, char **envp)
{
	int	id;

	if (middle_process(vars, argv, argc, envp) == -1)
		return ;
	vars->children_num += 1;
	if (access(argv[argc - 1], W_OK) == -1)
	{
		perror(argv[argc - 1])
		wait_for_all(vars);
		exit(0);
	}
	id = fork();
	if (!id)
		last_child(vars->pipefd, argv, argc, envp);
	else
	{
		close(vars->pipefd[1]);
		if (id == -1)
			ft_putstr_fd("fork failed\n", 2);
		close(vars->pipefd[0]);
	}
}

int	middle_process(t_pipex *vars, char *argv[], int argc, char **envp)
{
	int	id;
	int	arg;
	int	tmp_pipefd[2];

	arg = 3;
	while (arg + 2 < argc)
	{
		tmp_pipefd[1] = vars->pipefd[1];
		tmp_pipefd[0] = vars->pipefd[0];
		if (pipe(vars->pipefd) == -1)
			return (perror("Pipe failed"), -1);
		vars->children_num += 1;
		id = fork();
		if (!id)
			return (middle_child(argv[arg], tmp_pipefd, vars->pipefd, envp));
		else
		{
			close(tmp_pipefd[1]);
			close(tmp_pipefd[0]);
		}
		arg++;
	}
	return (1);
}

int	first_process(char *argv[], int argc, t_pipex *vars, char **envp)
{
	int	id;

	else
	{
		vars->children_num += 1;
		id = fork();
		if (!id)
		{
			if (access(argv[1], R_OK) == -1)
			{
				perror(argv[1]);
				exit(0);
			}
			first_child(vars->pipefd, argv, envp);
		}
		else
		{
			close(vars->pipefd[1]);
			if (>id == -1)
				ft_putstr_fd("fork failed\n", 2);
			last_process(argv, argc, vars, envp);
		}
	}
	return (0);
}

int	main(int argc, char *argv[], char **envp)
{
	int	fd;
	t_pipex	vars;

	if (argc < 5)
		return (write(2, "not enough inputs\n", 19), 1);
	if (pipe(vars.pipefd) == -1)
		return (perror("Pipe failed"), 3);
	if (access(argv[argc - 1], F_OK) == -1)
	{
		fd = open(argv[argc - 1], O_CREAT | O_WRONLY, 0644);
		if (fd != -1)
			close (fd);
	}
	vars.children_num = 0;
	if (ft_strncmp("here_doc", argv[1], 10) == 0)
		limiter_f(argv, argc, &vars, envp);
	else
		first_process(argv, argc, &vars, envp);
	wait_for_all(&vars);
	return (0);
}
