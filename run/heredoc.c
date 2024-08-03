/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojepar <seojepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 22:47:21 by seojepar          #+#    #+#             */
/*   Updated: 2024/08/03 22:06:54 by seojepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "run.h"

static void	do_sigint_heredoc(int signum)
{
	g_sig = signum;
	ft_putendl_fd("", 1);
	exit(1);
}

static void	init_term2(t_pipe *info, struct termios *term, char **line)
{
	tcgetattr(STDIN_FILENO, term);
	term->c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, term);
	signal(SIGINT, do_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
	*line = NULL;
}

static void	wait_heredoc(char **env, t_pipe *info, int pid)
{
	int	state;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &state, 0);
	state = WEXITSTATUS(state);
	if (state == 1)
	{
		g_sig = SIGINT;
		info->io_flag = TRUE;
		free(*env);
		*env = ft_strdup("?=1");
	}
	else if (state == 0)
	{
		free(*env);
		*env = ft_strdup("?=0");
	}
	set_signal();
}

static void	get_line_heredoc(t_redirect *redirect, int out)
{
	char	*line;

	while (1)
	{
		line = readline("> \0337");
		if (!line && (ft_putstr_fd("\0338", 1), 1))
			break ;
		if (ft_strcmp(line, redirect->file_path) == 0)
		{
			free(line);
			break ;
		}
		write(out, line, ft_strlen(line));
		write(out, "\n", 1);
		free(line);
	}
}

void	exec_heredoc(t_redirect *redirect, char **env, t_pipe *info)
{
	char			*line;
	struct termios	term;
	pid_t			pid;
	int				pfd[2];

	if (g_sig == SIGINT)
		return ;
	safe_pipe(pfd);
	pid = fork();
	if (pid == -1)
		pexit("fork failed");
	if (pid == 0)
	{
		restore_io(*info);
		init_term2(info, &term, &line);
		get_line_heredoc(redirect, pfd[W]);
		exit (0);
	}
	else
	{
		close(pfd[W]);
		dup2(pfd[R], STDIN_FILENO);
		close(pfd[R]);
		wait_heredoc(env, info, pid);
	}
}
