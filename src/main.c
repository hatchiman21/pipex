/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 21:45:11 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/07 14:19:55 by aatieh           ###   ########.fr       */
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
	exit(EXIT_SUCCESS);
}

void	middle_process(t_pipex *vars, char *argv[], int argc, char **envp)
{
	int	id;
	int	arg_num;
	int	tmp_pipefd[2];

	arg_num = 3;
	while (arg_num + 2 < argc)
	{
		tmp_pipefd[1] = vars->pipefd[1];
		tmp_pipefd[0] = vars->pipefd[0];
		if (pipe(vars->pipefd) == -1)
		{
			perror("middle Pipe failed");
			return ;
		}
		close(tmp_pipefd[1]);
		vars->children_num += 1;
		id = fork();
		if (!id)
			middle_child(argv[arg_num], tmp_pipefd, vars->pipefd, envp);
		else
		{
			close(vars->pipefd[1]);
			close(tmp_pipefd[0]);
		}
		arg_num++;
	}
}

void	last_process(char *argv[], int argc, t_pipex *vars, char **envp)
{
	int	id;

	middle_process(vars, argv, argc, envp);
	vars->children_num += 1;
	id = fork();
	if (!id)
	{
		if (access(argv[argc - 1], W_OK) == -1)
		{
			perror(argv[argc - 1]);
			exit(EXIT_FAILURE);
		}
		last_child(vars->pipefd, argv, argc, envp);
	}
	else
	{
		close(vars->pipefd[1]);
		if (id == -1)
			ft_putstr_fd("middle fork failed\n", 2);
		close(vars->pipefd[0]);
	}
}

void	first_process(char *argv[], int argc, t_pipex *vars, char **envp)
{
	int	id;

	vars->children_num += 1;
	id = fork();
	if (!id)
	{
		if (access(argv[1], R_OK) == -1)
		{
			perror(argv[1]);
			exit(EXIT_FAILURE);
		}
		first_child(vars->pipefd, argv, envp);
	}
	else
	{
		close(vars->pipefd[1]);
		if (id == -1)
			ft_putstr_fd("fork failed\n", 2);
		last_process(argv, argc, vars, envp);
	}
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
