/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_helper_functions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:15:44 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/10 21:06:40 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/bonus_pipex.h"

void	free_all(char *str, char **split)
{
	int	i;

	if (str)
		free(str);
	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	close_all(int fd1, int fd2)
{
	if (fd1 && fd1 != -1)
		close(fd1);
	if (fd2 && fd2 != -1)
		close(fd2);
}

int	wait_for_all(t_pipex *vars)
{
	int	last_status;
	int	pid;
	int	status;

	last_status = 0;
	while (vars->children_num > 0)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break ;
		if (pid == vars->last_id)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		vars->children_num -= 1;
	}
	return (last_status);
}

void	check_access(char *arg, t_pipex *vars, int is_first)
{
	if (access(arg, F_OK) == -1)
	{
		ft_dprintf(2, "pipex: no such file or directory: %s\n", arg);
		close_all(vars->pipefd[0], vars->pipefd[1]);
		exit(2);
	}
	if (is_first)
	{
		if (access(arg, R_OK) == -1)
		{
			ft_dprintf(2, "pipex: permission denied: %s\n", arg);
			close_all(vars->pipefd[0], vars->pipefd[1]);
			exit(2);
		}
	}
	else
	{
		if (access(arg, W_OK) == -1)
		{
			ft_dprintf(2, "pipex: permission denied: %s\n", arg);
			close_all(vars->pipefd[0], vars->pipefd[1]);
			exit(2);
		}
	}
}
