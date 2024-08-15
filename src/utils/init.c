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
	img->line_segment = NULL;
	return (img);
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

t_cam	*cam_init(t_map *data)
{
	t_cam	*cam;

	cam = (t_cam *)ft_calloc(1, sizeof(t_cam));
	if (NULL == cam)
		return (NULL);
	cam->projection = ISOMETRIC;
	cam->colour_pallet = true;
	cam->scale_factor = scale_to_fit(data);
	cam->scale_z = -0.16;
	cam->cam_position_x = WINDOW_WIDTH / 2;
	cam->cam_position_y = WINDOW_HEIGHT / 2;
	cam->alpha = ANG_30;
	cam->beta = 0.0;
	cam->gamma = ANG_45;
	cam->line_thickness = 1.5;
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
	move_origin(fdf->map_data);
	fdf->mlx_ptr = mlx_init();
	fdf->win_ptr = mlx_new_window(fdf->mlx_ptr, WINDOW_WIDTH, \
			WINDOW_HEIGHT, "fdf");
	fdf->img_ptr = img_init(fdf->mlx_ptr);
	fdf->cam_ptr = cam_init(fdf->map_data);
	if (!fdf->img_ptr || !fdf->cam_ptr)
		clean_up(&fdf);
	return (fdf);
}
