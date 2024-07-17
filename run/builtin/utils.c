/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojepar <seojepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 21:33:06 by seojepar          #+#    #+#             */
/*   Updated: 2024/07/17 22:09:38 by seojepar         ###   ########.fr       */
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
		builtin_cd(cmd->argv, *env);
	else if (ft_strcmp(cmd->file_path, "pwd") == 0)
		builtin_pwd(*env);
	else if (ft_strcmp(cmd->file_path, "export") == 0)
		builtin_export(cmd->argv, env, info);
	else if (ft_strcmp(cmd->file_path, "unset") == 0)
		builtin_unset(cmd->argv, env);
	else if (ft_strcmp(cmd->file_path, "env") == 0)
		builtin_env(cmd->argv, *env);
	else if (ft_strcmp(cmd->file_path, "exit") == 0)
		builtin_exit(cmd->argv, *env);
	else
		return (0);
	return (1);
}

void	pexit(char *msg)
{
	printf("%s", msg);
	exit(1);
}

void	wait_all_child(t_pipe *info, char **env)
{
	int		i;
	int		state;
	char	*exit_str;
	char	*tmp;

	i = 0;
	while (i < info->total_child_cnt)
	{
		waitpid(-1, &state, 0);
		i++;
	}
	if (WIFEXITED(state))
        state = WEXITSTATUS(state);
    else if (WIFSIGNALED(state))
        state = WTERMSIG(state);
	free(*env);
	tmp = ckm(ft_itoa(state % 256));
	*env = ckm(ft_strjoin("?=", tmp));
	free(tmp);
}

void	exec_command(t_tree *node, char ***env, t_pipe *info)
{
	t_simplecmd	*cmd;
	pid_t		pid;

	cmd = (t_simplecmd *)node->data;
	if (execute_builtin(cmd, env, info) == FALSE)
	{
		pid = fork();
		if (pid < 0)
			puterr_exit("Fork failed");
		if (pid == 0)
			exec_argv(cmd->file_path, cmd->argv, *env);
		else
		{
			info->total_child_cnt++;
			if (!info->next_pipe_exist)
				wait_all_child(info, *env);
		}
	}
}
