/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 15:47:50 by szhong            #+#    #+#             */
/*   Updated: 2024/08/16 15:53:14 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fdf.h"

float	max(float a, float b)
{
	if (a < b)
		return (b);
	else
		return (a);
}

float	min(float a, float b)
{
	if (a < b)
		return (a);
	else
		return (b);
}

float	scale_to_fit(t_map *data)
{
	float	scale_x;
	float	scale_y;
	float	scale_factor;

	scale_x = WINDOW_WIDTH / data->max_n;
	scale_y = WINDOW_HEIGHT / data->max_m;
	scale_factor = min(scale_x, scale_y);
	if (scale_factor < 4)
		return (2);
	return (scale_factor / 2);
}

t_line	*line_init(t_cartesian start, t_cartesian end, t_fdf *fdf)
{
	t_line	*line;

	line = malloc(sizeof(t_line));
	if (NULL == line)
		return (NULL);
	line->start.x = start.x;
	line->end.x = end.x;
	line->start.y = start.y;
	line->end.y = end.y;
	line->start.colour = start.colour;
	line->end.colour = end.colour;
	line->start.z = start.z;
	line->end.z = end.z;
	line->transform_z = max((fdf->map_data->max_z - fdf->map_data->min_z), \
			max(fdf->map_data->max_n, fdf->map_data->max_m));
	return (line);
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
