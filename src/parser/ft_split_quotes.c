/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbaud <gbaud@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/02 04:02:15 by gbaud             #+#    #+#             */
/*   Updated: 2020/09/19 11:00:48 by gbaud            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		**allocate_all(char *str, int (*f)(const char *, int))
{
	char	**res;
	int		i;
	int		j;

	if (!(res = ft_calloc((count_strs(str, f) + 1), sizeof(char *))))
		return (NULL);
	i = 0;
	j = -1;
	while (++j <= count_strs(str, f))
	{
		if (str[i])
			if (!(res[j] = ft_calloc(count_from(str, &i, f) + 1, 1)))
				return (ft_free_error(res, j));
		while (f(str, i))
		{
			if (++j < count_strs(str, f))
				if (!(res[j] = ft_calloc(f(str, i) + 1, 1)))
					return (ft_free_error(res, j));
			i += f(str, i);
		}
	}
	return (res);
}

t_boolean	quotes_are_valid(const char *str)
{
	int				i;
	t_split_quotes	states;

	i = 0;
	states.d_quote = 0;
	states.s_quote = 0;
	states.escaped = 0;
	while (str[i])
	{
		check_quotes(&states, str, i);
		states.escaped = (str[i] == '\\') ? states.escaped + 1 : 0;
		i++;
	}
	if (states.d_quote || states.s_quote)
		return (FALSE);
	return (TRUE);
}

void		ft_fill_tab(char **res, char *str, int (*f)(const char *, int))
{
	int i;
	int j;

	i = 0;
	j = -1;
	while (++j < count_strs(str, f))
	{
		fill_from(res[j], str, &i, f);
		while (f(str, i))
		{
			if (++j < count_strs(str, f))
			{
				res[j][0] = str[i];
				if (f(str, i) == 2)
					res[j][1] = str[i + 1];
				i += f(str, i);
			}
		}
	}
}

void		fill_from(char *res, char *str, int *i,
					int (*f)(const char *, int))
{
	t_split_quotes	states;
	int				j;

	j = 0;
	states.d_quote = 0;
	states.s_quote = 0;
	states.escaped = 0;
	while (str[*i])
	{
		check_quotes(&states, str, *i);
		if (!states.d_quote && !states.s_quote && f(str, *i))
			break ;
		res[j++] = str[*i];
		states.escaped = (str[*i] == '\\') ? states.escaped + 1 : 0;
		(*i)++;
	}
}

char		**ft_split_quotes(char *str, int (*f)(const char *, int))
{
	char **res;

	if (!(res = allocate_all(str, f)))
		return (NULL);
	ft_fill_tab(res, str, f);
	res[count_strs(str, f)] = NULL;
	return (res);
}
