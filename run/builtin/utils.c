/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojepar <seojepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 21:33:06 by seojepar          #+#    #+#             */
/*   Updated: 2024/08/03 18:37:57 by seojepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "run.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	execute_builtin(t_simplecmd *cmd, char ***env, t_pipe *info)
{
	if (ft_strcmp(cmd->file_path, "echo") == 0)
		builtin_echo(cmd->argv, *env);
	else if (ft_strcmp(cmd->file_path, "cd") == 0)
		builtin_cd(cmd->argv, env);
	else if (ft_strcmp(cmd->file_path, "pwd") == 0)
		builtin_pwd(*env);
	else if (ft_strcmp(cmd->file_path, "export") == 0)
		builtin_export(cmd->argv, env, info);
	else if (ft_strcmp(cmd->file_path, "unset") == 0)
		builtin_unset(cmd->argv, env);
	else if (ft_strcmp(cmd->file_path, "env") == 0)
		builtin_env(cmd->argv, *env);
	else if (ft_strcmp(cmd->file_path, "exit") == 0)
		builtin_exit(cmd->argv, *env, info);
	else
		return (0);
	return (1);
}

void	pexit(char *msg)
{
	ft_putstr_fd(msg, 2);
	exit(1);
}

void	wait_all(t_pipe *info, int *state)
{
	int	i;

	i = 0;
	while (i < info->total_child_cnt)
	{
		waitpid(-1, state, 0);
		i++;
	}
	if (WIFEXITED(*state))
		*state = WEXITSTATUS(*state);
	else if (WIFSIGNALED(*state))
	{
		if (WTERMSIG(*state) == SIGINT)
			*state = 130;
		else if (WTERMSIG(*state) == SIGQUIT)
			*state = 131;
		else
			*state = 128 + *state;
	}
}

void	wait_all_child(t_pipe *info, char **env)
{
	int		state;
	char	*tmp;

	if (info->total_child_cnt == 0 || g_sig == SIGINT)
		return ;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	wait_all(info, &state);
	free(*env);
	tmp = ckm(ft_itoa(state % 256));
	*env = ckm(ft_strjoin("?=", tmp));
	free(tmp);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
