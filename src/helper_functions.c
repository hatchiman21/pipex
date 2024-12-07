/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:15:44 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/07 21:31:40 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	free_all(char *str, char **split)
{
	if (str)
		free(str);
	free_split(split);
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

void	free_split(char **string)
{
	int	i;

	i = 0;
	if (!string)
		return ;
	while (string[i])
	{
		free(string[i]);
		i++;
	}
	free(string);
}

char	*get_path(char *arg, char **envp)
{
	char	**paths;
	char	*path;
	char	*tmp;
	int		i;

	if (access(arg, X_OK) == 0)
		return (ft_strdup(arg));
	tmp = ft_strjoin("/", arg);
	paths = get_all_paths(envp);
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], tmp);
		if (access(path, X_OK) == 0)
			break ;
		free(path);
		if (!paths[i + 1])
			path = ft_strjoin(".", tmp);
		i++;
	}
	free_split(paths);
	free(tmp);
	if (!path)
		path = NULL;
	return (path);
}
