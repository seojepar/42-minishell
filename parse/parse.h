/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungcho <hyungcho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 04:23:19 by hyungcho          #+#    #+#             */
/*   Updated: 2024/07/07 19:18:34 by hyungcho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "../minishell.h"

/* replvar.c */
char	*replace_variable(char *str);

/* token.c */
void	tokenize(t_list **token_list, char *str);
void	delete_token(void *token);

/* split.c */
char	**var_split(const char *str);

/* quote.c */
int		check_quote(const char *str, int index);

/* treebuilder */
t_tree	*syntax_pipeline(t_list *token_list);
t_tree	*syntax_cmd(t_list *token_list);
t_tree	*syntax_redirects(t_list *token_list);
t_tree	*syntax_io_redirect(t_list *token_list);
t_tree	*syntax_simple_cmd(t_list *token_list);

#endif
