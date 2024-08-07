/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 parse_line.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/07/29 16:16:59 by szhong			   #+#	  #+#			  */
/*	 Updated: 2024/07/29 16:23:46 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "fdf.h"
#include "libft.h"

static void	parse_z_and_colour(char *token, t_cartesian *point, t_map *data)
{
	char	*comma;

	comma = ft_strchr(token, ',');
	if (comma)
	{
		*comma = '\0';
		point->z = (float)ft_atoi(token);
		point->colour = ft_atoi_base(comma + 1, HEXADECIMAL);
	}
	else
	{
		point->z = (float)ft_atoi(token);
		point->colour = -1;
	}
	if (point->z > data->max_z)
		data->max_z = point->z;
	if (point->z < data->min_z)
		data->min_z = point->z;
}

static void	parse_line(char *line, t_cartesian *point, int row, t_map *data)
{
	char	**split;
	int		col;

	split = ft_split(line, ' ');
	if (!split)
		return ;
	col = 0;
	while (split[col] && col < data->max_n && point[col].x != -1)
	{
		point[col].x = (float)col;
		point[col].y = (float)row;
		parse_z_and_colour(split[col], &point[col], data);
		col++;
	}
	free_arr(split);
}

void	parse_line_wrapper(char *line, t_map *data)
{
	parse_line(line, data->points[data->current_row], data->current_row, data);
	data->current_row++;
}
