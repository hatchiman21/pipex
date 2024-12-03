/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 21:43:21 by aatieh            #+#    #+#             */
/*   Updated: 2024/11/12 16:32:54 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "./libft/libft.h"
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>

int		creat_tmp_fd(int pipefd[2]);
void	free_split(char **string);
char	**get_cmd(char *arg);
int		dub_failed(int fd1, int fd2);

int		first_child(int pipefd[2], char *argv[], char **envp);
int		middle_child(char *arg, int tmp_fd, int pipefd[2], char **envp);
int		last_child(int pipefd[2], char *argv[], int argc, char **envp);
int		limiter_f(char *argv[], int argc, char **envp);

#endif