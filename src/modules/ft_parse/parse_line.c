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
#include <limits.h>

static int	is_valid_z(const char *str)
{
	long long	num;
	int			i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (PARSE_FAILURE);
		i++;
	}
	num = ft_atol(str);
	if (num > LONG_MAX || num < LONG_MIN)
		return (PARSE_FAILURE);
	return (PARSE_SUCCESS);
}

static void	parse_z(char *token, t_cartesian *point, t_map *data)
{
	if (!is_valid_z(token))
	{
		data->parse_error = TRUE;
		return ;
	}
	point->z = (float)ft_atoi(token);
	point->colour = -1;
}

static void	parse_z_and_colour(char *token, t_cartesian *point, t_map *data)
{
	char	*comma;

	comma = ft_strchr(token, ',');
	if (comma)
	{
		*comma = '\0';
		parse_z(token, point, data);
		if (!data->parse_error)
			point->colour = ft_atoi_base(comma + 1, HEXADECIMAL);
	}
	else
		parse_z(token, point, data);
	if (!data->parse_error)
	{
		data->max_z = fmaxf(data->max_z, point->z);
		data->min_z = fminf(data->min_z, point->z);
	}
}

static void	parse_line(char *line, t_cartesian *point, int row, t_map *data)
{
	char	**split;
	int		col;

	split = ft_split(line, ' ');
	if (!split)
	{
		data->parse_error = TRUE;
		return ;
	}
	col = 0;
	while (split[col] && col < data->max_n && point[col].x != -1 && \
			!data->parse_error)
	{
		point[col].x = (float)col;
		point[col].y = (float)row;
		parse_z_and_colour(split[col], &point[col], data);
		col++;
	}
	free_arr(split);
	if (col < data->max_n || (col == data->max_n && split[col] != NULL))
		data->parse_error = TRUE;
	return ;
}

void	parse_line_wrapper(char *line, t_map *data)
{
	parse_line(line, data->points[data->current_row], data->current_row, data);
	data->current_row++;
}
