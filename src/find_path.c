/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 18:06:10 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/22 18:33:50 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

char	**get_all_paths(char **envp)
{
	int		i;
	char	**path;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	path = ft_split(envp[i] + 5, ':');
	return (path);
}

char	*get_final_path(char **paths, char *tmp)
{
	char	*path;
	char	**tmp_path;

	if (!paths || !tmp)
		return (NULL);
	tmp_path = paths;
	path = NULL;
	while (tmp_path && tmp_path[0])
	{
		path = ft_strjoin(*tmp_path, tmp);
		if (!path)
			break ;
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		path = NULL;
		tmp_path++;
	}
	return (path);
}

char	*get_path(char **cmd, char **envp)
{
	char	**paths;
	char	*path;
	char	*tmp;

	if (access(cmd[0], X_OK) == 0)
		return (ft_strdup(cmd[0]));
	tmp = ft_strjoin("/", cmd[0]);
	paths = get_all_paths(envp);
	if (!paths || !tmp)
	{
		free_all(tmp, cmd);
		return (NULL);
	}
	path = get_final_path(paths, tmp);
	free_all(tmp, paths);
	if (path == NULL)
	{
		ft_dprintf(2, "pipex: command not found: %s\n", cmd[0]);
		free_all(NULL, cmd);
		exit(127);
	}
	return (path);
}
