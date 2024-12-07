/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 21:45:11 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/07 21:35:45 by aatieh           ###   ########.fr       */
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

void	middle_process(char *argv[], int argc, t_pipex *vars, char **envp)
{
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
		vars->last_id = fork();
		if (!vars->last_id)
			middle_child(argv[arg_num], tmp_pipefd, vars->pipefd, envp);
		else
			close_all(vars->pipefd[1], tmp_pipefd[0]);
		arg_num++;
	}
}

void	last_process(char *argv[], int argc, t_pipex *vars, char **envp)
{
	middle_process(argv, argc, vars, envp);
	vars->children_num += 1;
	vars->last_id = fork();
	if (!vars->last_id)
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
		if (vars->last_id == -1)
			ft_putstr_fd("middle fork failed\n", 2);
		close(vars->pipefd[0]);
	}
}

void	first_process(char *argv[], int argc, t_pipex *vars, char **envp)
{
	vars->children_num += 1;
	vars->last_id = fork();
	if (!vars->last_id)
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
		if (vars->last_id == -1)
			ft_putstr_fd("fork failed\n", 2);
		last_process(argv, argc, vars, envp);
	}
}

int	main(int argc, char *argv[], char **envp)
{
	int		fd;
	t_pipex	vars;

	if (argc < 5)
	{
		ft_dprintf(2, "Not enough input\n");
		return (1);
	}
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
		limiter_process(argv, argc, &vars, envp);
	else
		first_process(argv, argc, &vars, envp);
	return (wait_for_all(&vars));
}
