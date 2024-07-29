/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:59:19 by szhong            #+#    #+#             */
/*   Updated: 2024/07/29 15:32:38 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <link.h>

#include <fcntl.h>
#include "libft.h"
#include "fdf.h"
#include <assert.h> 
#include <stdio.h>

#include "../minilibx-linux/mlx.h"

#define WINDOW_WIDTH 1680 
#define WINDOW_HEIGHT 980

void	draw_pixel(t_fdf *data, int x, int y, int color)
{
	mlx_pixel_put(data->mlx_ptr, data->win_ptr, x, y, color);
}

//int	calculate_color(int x, int y)
//{
//	float	progress;
//	int	red, green, blue;
//	float	tmp;
//
//	progress =(float )(x + y) / (WINDOW_WIDTH + WINDOW_HEIGHT - 2);
//	red = (int)(255 * progress);
//	green = (int)(255 * (1 - progress));
//	tmp = 0.5 - progress;
//	if (tmp < 0)
//		tmp = -tmp;
//	blue = (int)(255 * tmp * 2);
//	return ((red << 16) | (green << 8)| blue);
//}

void	draw_diagonal(t_fdf *data)
{
	int	x, y;
	int	color;
	float	slope;


	slope = (float )(WINDOW_HEIGHT - 1) / (WINDOW_WIDTH - 1);
	x = 0;
	while (x < WINDOW_WIDTH )
	{
		y = (int)(x * slope);
		if ( y < WINDOW_HEIGHT)
		{
			//color = calculate_color(x, y);
			color = ft_atoi_base("0x00FFFFFF", HEXADECIMAL);
			draw_pixel(data, x , y , color);
		}
		x++;
	}
}

int	main(int argc, char *argv[])
{
	t_fdf	*fdf;

	if (argc != 2)
		ft_putendl_fd("ERROR", 2);
	fdf = (t_fdf *)malloc(sizeof(t_fdf) * 1);
	if (fdf == NULL)
		return (-1);
	fdf->map_data = parse_data(argv[1]);

	if (fdf->map_data == NULL)
		return (-1);
//	fdf->mlx_ptr = mlx_init();
//	if (NULL == fdf->mlx_ptr)
//	{
//		free_points(fdf->map_data->points, fdf->map_data->max_m);
//		free(fdf->map_data);
//		return (-1);
//	}
//	fdf->win_ptr = mlx_new_window(fdf->mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, "TESTING");
//	if (NULL == fdf->win_ptr)
//	{
//		mlx_destroy_display(fdf->mlx_ptr);
//		free(fdf->mlx_ptr);
//		free_points(fdf->map_data->points, fdf->map_data->max_m);
//		free(fdf->map_data);
//	}
//
//	draw_diagonal(fdf);
//	mlx_loop(fdf->mlx_ptr);
//
	print_map(fdf->map_data->points, fdf->map_data->max_n, fdf->map_data->max_m);
//	// free options
//	mlx_destroy_window(fdf->mlx_ptr,fdf->win_ptr);
//	mlx_destroy_display(fdf->mlx_ptr);
//	free(fdf->mlx_ptr);
	free_points(fdf->map_data->points, fdf->map_data->max_m);
	free(fdf->map_data);
	free(fdf);
}
