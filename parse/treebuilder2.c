/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   treebuilder2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungcho <hyungcho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 16:36:44 by hyungcho          #+#    #+#             */
/*   Updated: 2024/07/14 17:03:57 by hyungcho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static int		get_redirection_type(char *str);
static char		**get_argv(t_list *token_list);
static t_list	*get_cmd_token_list(t_list *token_list);

/*
 * token_list는 pipe, redirect, null 중 하나
 * token_list의 첫 번째 token은 type
 * token_list의 두 번째 token은 file_path
 * 세 번째 인자부터 처리 x
 */
t_tree	*syntax_io_redirect(t_list *token_list)
{
	t_tree		*tree;
	t_redirect	*redirect;
	t_token		*token;

	if (token_list == NULL
		|| ((t_token *)token_list->content)->type != T_REDIRECT)
		return (NULL);
	if (token_list->next == NULL
		|| ((t_token *)token_list->next->content)->type != T_WORD)
		return (puterr("wrong redirect input"));
	redirect = (t_redirect *)xmalloc(sizeof(t_redirect));
	token = (t_token *)token_list->content;
	redirect->type = get_redirection_type(token->str);
	token = (t_token *)token_list->next->content;
	redirect->file_path = ft_strdup(token->str);
	tree = (t_tree *)xmalloc(sizeof(t_tree));
	tree->type = T_REDIRECT;
	tree->left = NULL;
	tree->right = NULL;
	tree->data = redirect;
	return (tree);
}

/*
 * token_list는 pipe, redirect, null 중 하나
 * token_list의 첫 번째 token은 type
 * token_list의 첫 번째 token 이후는 argv
 */
t_tree	*syntax_simple_cmd(t_list *token_list)
{
	t_tree		*tree;
	t_simplecmd	*cmd;
	t_token		*token;

	if (token_list == NULL || ((t_token *)token_list->content)->type != T_WORD)
		return (puterr("wrong cmd input"));
	cmd = (t_simplecmd *)xmalloc(sizeof(t_simplecmd));
	token_list = get_cmd_token_list(token_list);
	token = (t_token *)token_list->content;
	cmd->file_path = ft_strdup(token->str);
	cmd->argv = get_argv(token_list);
	tree = (t_tree *)xmalloc(sizeof(t_tree));
	tree->type = T_SIMPLECMD;
	tree->left = NULL;
	tree->right = NULL;
	tree->data = cmd;
	return (tree);
}

/* safe */
char	**get_argv(t_list *token_list)
{
	int		count;
	int		i;
	char	**argv;
	t_list	*ptr;

	ptr = token_list->next;
	count = 2;
	while (ptr != NULL && ((t_token *)ptr->content)->type == T_WORD)
	{
		ptr = ptr->next;
		count++;
	}
	argv = (char **)xmalloc(sizeof(char *) * count);
	i = -1;
	while (++i < count - 1)
	{
		argv[i] = ckm(ft_strdup(((t_token *)token_list->content)->str));
		token_list = token_list->next;
	}
	argv[i] = NULL;
	return (argv);
}

t_list	*get_cmd_token_list(t_list *token_list)
{
	t_token	*token;

	while (token_list && !ft_strncmp(((t_token *)token_list->content)->str, "env", 4))
	{
		if (token_list->next)
			token = token_list->next->content;
		else
			break ;
		if (!(ft_strncmp(token->str, "echo", 5) == 0
			|| ft_strncmp(token->str, "pwd", 4) == 0
			|| ft_strncmp(token->str, "env", 4) == 0
			|| ft_strncmp(token->str, "cd", 3) == 0
			|| ft_strncmp(token->str, "export", 7) == 0
			|| ft_strncmp(token->str, "unset", 6) == 0))
			break ;
		token_list = token_list->next;
	}
	return (token_list);
}

/* safe */
int	get_redirection_type(char *str)
{
	if (ft_strncmp(str, ">", 2) == 0)
		return (OUTPUT_REDIRECT);
	if (ft_strncmp(str, ">>", 2) == 0)
		return (APPEND_REDIRECT);
	if (ft_strncmp(str, "<", 2) == 0)
		return (INPUT_REDIRECT);
	if (ft_strncmp(str, "<<", 2) == 0)
		return (HERE_DOCUMENT);
	puterr_exit("get_redirection_type failed");
	return (FAILURE);
}
