/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 21:43:21 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/07 19:58:52 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "./libft/libft.h"
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>

typedef struct s_pipex
{
	int	pipefd[2];
	int	children_num;
	int	last_id;
}				t_pipex;

char	**get_all_paths(char **envp);
void	free_split(char **string);
char	*get_path(char *arg, char **envp);
int		wait_for_all(t_pipex *vars);
void	close_all(int fd1, int fd2);
void	free_all(char *str, char **split);

int		first_child(int pipefd[2], char *argv[], char **envp);
int		middle_child(char *arg, int tmp_pipefd[2], int pipefd[2], char **envp);
int		last_child(int pipefd[2], char *argv[], int argc, char **envp);

void	first_process(char *argv[], int argc, t_pipex *vars, char **envp);
void	middle_process(char *argv[], int argc, t_pipex *vars, char **envp);
void	last_process(char *argv[], int argc, t_pipex *vars, char **envp);
void	limiter_process(char *argv[], int argc, t_pipex *vars, char **envp);

#endif
