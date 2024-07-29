/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 13:30:01 by szhong            #+#    #+#             */
/*   Updated: 2024/07/29 15:14:47 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fdf.h"
#include "libft.h"
/*
 * this function is actually quite smart
 * it actually uses a lot of other functions to work on the same issues
 *
 * for example, we have `ft_strlen`, it will stop when it has the `\0` null terminator
 * also the return value of `ft_strchr` is NULL when the targeted character isn't found
 * if  statement will not run if the condition is not ture,
 * so if the `ptr_nextline` was found by the `ft_strchr` then it won't be a NULL
 * so these two conditions will be true. 
 */
static void	count_dimensions(char *content, t_map *data)
{
	
	char		*line;
	char		*ptr_nextline;
	size_t		line_length;

	line = content;
	
	while (line && *line)
	{
		ptr_nextline = ft_strchr(line, '\n');
		if (ptr_nextline)
		{
			line_length = ptr_nextline - line;
			*ptr_nextline = '\0';
		}
		else
			line_length = ft_strlen(line);
		if (line_length > 0 || data->max_m == 0)
		{
			if (data->max_m == 0)
				data->max_n = count_cols(line, ' ');
			data->max_m++;
		}
		if (ptr_nextline)
		{
			*ptr_nextline = '\n';
			line = ptr_nextline + 1;
		}
		else
			break ;
	}
}

static void parse_z_and_colour(char *token, t_cartesian *point, t_map *data)
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
	int	col;

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

static void	parse_content(char *content, t_map *data)
{
	char		*line;
	char		*ptr_nextline;
	int		row;
	size_t		line_length;

	line = content;
	row = 0;
	while (line && *line && row < data->max_m)
	{
		ptr_nextline = ft_strchr(line, '\n');
		if (ptr_nextline)
		{
    			line_length = ptr_nextline - line;
			*ptr_nextline = '\0';
		}
		else
			line_length = ft_strlen(line);
		if (line_length > 0 || row == 0)
		{
			parse_line(line, data->points[row], row, data);
			row++;
		}
		if (ptr_nextline)
		{
			*ptr_nextline = '\n';
			line = ptr_nextline + 1;
		}
		else
			break;
	}
}

t_map	*parse_data(char *filepath)
{
	t_map	*data;
	char	*file_content;

	data = map_init();
	if (!data)
		return (NULL);
	file_content = get_whole_file(filepath);
	if (!file_content)
	{
		free(data);
		return (NULL);
	}
	count_dimensions(file_content, data);
	data->points = cartesian_init(data->max_n, data->max_m);
	if (!data->points)
	{
		free(file_content);
		free(data);
		return (NULL);
	}
	parse_content(file_content, data);
	free(file_content);
	return (data);
}
