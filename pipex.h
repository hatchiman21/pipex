/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 21:43:21 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/05 20:21:06 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "./libft/libft.h"
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>

typedef struct	s_child
{
	int				id;
	struct s_child	*next;
}				t_child;

typedef struct	s_pipex
{
	int		pipefd[2];
	t_child	*first_child;
}				t_pipex;


char	**get_all_paths(char **envp);
int		creat_tmp_fd(int pipefd[2]);
void	free_split(char **string);
char	*get_path(char *arg, char **envp);
void	print_path(char **path);
int		dub_failed(int fd1, int fd2);

int		first_child(int pipefd[2], char *argv[], char **envp);
int		middle_child(char *arg, int tmp_pipefd[2], int pipefd[2], char **envp);
int		last_child(int pipefd[2], char *argv[], int argc, char **envp);
int		limiter_f(char *argv[], int argc, t_pipex *vars, char **envp);

#endif