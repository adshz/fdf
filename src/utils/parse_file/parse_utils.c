/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 parse_utils.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/07/29 16:19:07 by szhong			   #+#	  #+#			  */
/*	 Updated: 2024/08/01 11:50:32 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "fdf.h"

int	ft_arrlen(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

size_t	count_cols(char const *s, char c)
{
	size_t	word_count;

	word_count = 0;
	while (s && *s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			while (*s && *s != c)
				s++;
			word_count++;
		}
	}
	return (word_count);
}
