/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojepar <seojepar@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 21:13:55 by seojepar          #+#    #+#             */
/*   Updated: 2024/07/08 23:49:23 by hyungcho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 *  1. 데이터 초기화 (환경변수, 들고다닐 데이터변수)
 *  2. 프롬프트 찍기
 *  3. while문 실행
 *      -> 문자열 읽기
 *      -> 파싱하여 토큰화 하기
 *      -> 파싱한 데이터 검사하기
 *      -> 토큰화한 데이터로 실행시키기
 */

static void	set_term(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		puterr_exit("tcgetattr failed");
	term.c_lflag &= ~(ECHOCTL);
	if (tcsetattr(STDIN_FILENO, TCSADRAIN, &term) == -1)
		puterr_exit("tcsetattr failed");
}

static void	set_g_envp(char **envp)
{
	int		i;

	i = -1;
	while (envp[++i])
		;
	g_envp = (char **)xmalloc(sizeof(char *) * (i + 2));
	g_envp[0] = ckm(ft_strdup(""));
	i = -1;
	while (envp[++i])
		g_envp[i + 1] = ckm(ft_strdup(envp[i]));
	g_envp[i + 1] = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	set_g_envp(envp);
	set_term();
	set_signal();
	start_shell();
	exit(0);
}
