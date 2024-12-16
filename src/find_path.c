/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 18:06:10 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/16 16:42:51 by aatieh           ###   ########.fr       */
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

char	*get_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	char	*tmp;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	tmp = ft_strjoin("/", cmd);
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
