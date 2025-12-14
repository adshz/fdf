/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 14:19:33 by szhong            #+#    #+#             */
/*   Updated: 2024/08/01 18:02:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fdf.h"
#include "libft.h"
#include <math.h>
#include <limits.h>

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
	result->parse_error = FALSE;
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

t_img	*img_init(void *mlx_ptr)
{
	t_img		*img;
	mlx_image_t	*mlx_img;
	mlx_t		*mlx;

	mlx = (mlx_t *)mlx_ptr;
	img = (t_img *)ft_calloc(1, sizeof(t_img));
	if (img == NULL)
		return (NULL);
	mlx_img = mlx_new_image(mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!mlx_img)
	{
		free(img);
		return (NULL);
	}
	img->img = mlx_img;
	img->line_segment = NULL;
	if (mlx_image_to_window(mlx, mlx_img, 0, 0) < 0)
	{
		mlx_delete_image(mlx, mlx_img);
		free(img);
		return (NULL);
	}
	return (img);
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

t_fdf	*fdf_init(char *filepath)
{
	t_fdf	*fdf;

	fdf = (t_fdf *)ft_calloc(1, sizeof(t_fdf));
	if (fdf == NULL)
		error_handler(3);
	fdf->map_data = parse_data(filepath);
	if (!fdf->map_data)
	{
		free(fdf);
		error_handler(4);
	}
	move_origin(fdf->map_data);
	fdf->mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "fdf", true);
	if (!fdf->mlx)
	{
		free_points(fdf->map_data->points, fdf->map_data->max_m);
		free(fdf->map_data);
		free(fdf);
		error_handler(3);
	}
	fdf->img_ptr = img_init(fdf->mlx);
	fdf->cam_ptr = cam_init(fdf->map_data);
	if (!fdf->img_ptr || !fdf->cam_ptr)
		clean_up(&fdf);
	return (fdf);
}
