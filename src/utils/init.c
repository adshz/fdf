/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 init.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/07/29 14:19:33 by szhong			   #+#	  #+#			  */
/*	 Updated: 2024/08/01 18:02:23 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "fdf.h"
#include "libft.h"
#include <math.h>
# define MIN_SCALE 3
# define SCALE_DIVISOR 1.5

float	max(float a, float b)
{
	if (a < b)
		return (b);
	else
		return (a);
}

t_map	*map_init(void)
{
	t_map	*result;

	result = (t_map *)malloc(sizeof(t_map) * 1);
	if (!result)
		return (NULL);
	result->max_m = 0;
	result->max_n = 0;
	result->max_z = 0;
	result->min_z = 0;
	result->points = NULL;
	result->current_row = 0;
	return (result);
}

t_cartesian	**cartesian_init(int max_width, int max_depth)
{
	t_cartesian		**points;
	int				i;

	points = (t_cartesian **)ft_calloc(max_depth + 1, sizeof(t_cartesian *));
	if (points == NULL)
		return (NULL);
	i = 0;
	while (i < max_depth)
	{
		points[i] = ft_calloc(max_width + 1, sizeof(t_cartesian));
		if (!points[i])
		{
			free_points(points, max_depth + 1);
			return (NULL);
		}
		points[i][max_width].x = -1;
		i++;
	}
	points[max_depth] = NULL;
	return (points);
}

t_img	*img_init(void	*mlx)
{
	t_img	*img;

	img = (t_img *)ft_calloc(1, sizeof(t_img));
	if (img == NULL)
		return (NULL);
	img->img_buff = mlx_new_image(mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	img->mem_addr = mlx_get_data_addr(img->img_buff, &img->bits_per_pixel, \
			&img->line_len, &img->endian);
	img->line = NULL;
	return (img);
}

float	get_render_scale(t_map *data)
{
	float	scale_x;
	float	scale_y;
	float	scale;
	float	adjusted_scale;

	scale_x = WINDOW_WIDTH / data->max_n;
	scale_y = WINDOW_HEIGHT / data->max_m;
	if (scale_x < scale_y)
		scale = scale_x;
	else
		scale = scale_y;
	if (scale < MIN_SCALE)
		adjusted_scale = MIN_SCALE;
	else
		adjusted_scale = scale / SCALE_DIVISOR;
	adjusted_scale = floor(adjusted_scale);
	return (adjusted_scale);
}

t_cam	*cam_init(t_map *data)
{
	t_cam	*cam;

	cam = (t_cam *)ft_calloc(1, sizeof(t_cam));
	if (NULL == cam)
		return (NULL);
	cam->projection = 1;
	cam->colour_pallet = false;
	cam->scale_factor = get_render_scale(data);
	cam->scale_z = 1;
	cam->cam_position_x = WINDOW_WIDTH / 2;
	cam->cam_position_y = WINDOW_HEIGHT / 2;
	cam->alpha = 0;
	cam->beta = 0;
	cam->gamma = 0;
	return (cam);
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

t_fdf	*fdf_init(char	*filepath)
{
	t_fdf	*fdf;

	fdf = (t_fdf *)ft_calloc(1, sizeof(t_fdf));
	if (fdf == NULL)
	{
		perror("fdf initialisation failed");
		exit(1);
	}
	fdf->map_data = parse_data(filepath);
	fdf->map_data = move_origin(fdf->map_data);
	fdf->mlx_ptr = mlx_init();
	fdf->win_ptr = mlx_new_window(fdf->mlx_ptr, WINDOW_WIDTH , WINDOW_HEIGHT, "TESTING");
	fdf->img_ptr = img_init(fdf->mlx_ptr);
	fdf->cam_ptr = cam_init(fdf->map_data);
	clean_up(&fdf);
	return (fdf);
}
