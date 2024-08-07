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

void	move_origin(t_map *data)
{
	int	abc_x;
	int	abc_y;

	abc_y = 0;
	while (abc_y < data->max_m)
	{
		abc_x = 0;
		while (abc_x < data->max_n)
		{
			data->points[abc_y][abc_x].x -= data->max_n / 2;
			data->points[abc_y][abc_x].y -= data->max_m / 2;
			abc_x++;
		}
		abc_y++;
	}
}
