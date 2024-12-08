/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:15:44 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/08 19:02:41 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

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

char	*get_final_path(char **paths, char *tmp)
{
	int		i;
	char	*path;

	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], tmp);
		if (!path)
			break ;
		if (access(path, X_OK) == 0)
			break ;
		free(path);
		if (!paths[i + 1])
			path = ft_strjoin(".", tmp);
		i++;
	}
	if (!path)
		path = NULL;
	return (path);
}

char	*get_path(char *arg, char **envp)
{
	char	**paths;
	char	*path;
	char	*tmp;

	if (access(arg, X_OK) == 0)
		return (ft_strdup(arg));
	tmp = ft_strjoin("/", arg);
	paths = get_all_paths(envp);
	if (!paths || !tmp)
	{
		free_all(tmp, paths);
		return (NULL);
	}
	path = get_final_path(paths, tmp);
	free_all(tmp, paths);
	return (path);
}
