/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:59:19 by szhong            #+#    #+#             */
/*   Updated: 2024/07/30 16:56:24 by szhong           ###   ########.fr       */
/*                                                                            */ 
/* ************************************************************************** */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <link.h>
#include <math.h>
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

//void	render_data(t_fdf *fdf)
//{
//	int	row;
//	int	col;
//	float	draw_x;
//	float	draw_y;
//	int	color;
//	int	scale;
//	int	iso_x;
//	int	iso_y;
//	int	offset_x;	
//	int	offset_y;
//
//	row = 0;
//	offset_x = WINDOW_WIDTH / 2;
//	offset_y = WINDOW_HEIGHT / 2;
//	scale = 30;
//	color = 0xFF0000;
//	while (row < fdf->map_data->max_m)
//	{
//		col = 0;
//		while (col < fdf->map_data->max_n)
//		{
//			int z  = fdf->map_data->points[row][col].z;
//			iso_x = (col - row) * cos(0.523599);
//			iso_y = -z + (col + row) * sin(0.523599);
//			draw_x = iso_x * scale + offset_x;
//			draw_y = iso_y * scale + offset_y;
//			printf("Drawing point at (%f, %f) with color %d\n", draw_x, draw_y, color);
//			mlx_pixel_put(fdf->mlx_ptr, fdf->win_ptr, draw_x, draw_y, color);
//			col++;
//		}
//		row++;
//	}
//}

// it is a great function understand how the draw descrete points from parsed data
//void render_data(t_fdf *fdf)
//{
//    int row;
//    int col;
//    int scale;
//    int offset_x;
//    int offset_y;
//    int x;
//    int y;
//    int z;
//    int iso_x;
//    int iso_y;
//    int draw_x;
//    int draw_y;
//    int color;
//
//    row = 0;
//    scale = 30;
//    offset_x = WINDOW_WIDTH / 2;
//    offset_y = WINDOW_HEIGHT / 4;
//
//    while (row < fdf->map_data->max_m)
//    {
//        col = 0;
//        while (col < fdf->map_data->max_n)
//        {
//            x = col * scale;
//            y = row * scale;
//            z = fdf->map_data->points[row][col].z * scale / 10;
//            iso_x = (x - y) * cos(0.523599);
//            iso_y = -z + (x + y) * sin(0.523599);
//            draw_x = iso_x + offset_x;
//            draw_y = iso_y + offset_y;
//            if (z == 0)
//                color = 0xFFFFFF;
//            else
//                color = 0xFF0000;
//            mlx_pixel_put(fdf->mlx_ptr, fdf->win_ptr, draw_x, draw_y, color);
//            col++;
//        }
//        row++;
//    }
//}

void	draw_line(t_fdf *fdf, int x0, int y0, int x1, int y1, int color)
{
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	e2;

	dx = abs(x1 - x0);
	dy = abs(y1 - y0);
	sx = -1;
	sy = -1;
	if (x0 < x1)
		sx = 1;
	if (y0 < y1)
		sy = 1;
	err = dx - dy;
	while (1)
	{
		mlx_pixel_put(fdf->mlx_ptr, fdf->win_ptr, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break ;
		e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}

	}
}

void	iso_project(int x, int y, int z, int *iso_x, int *iso_y, int offset_x, int offset_y)
{
	*iso_x = (x - y) * cos(0.523599) + offset_x;
	*iso_y = -z + (x + y) * sin(0.523599) + offset_y;
}

void draw_horizontal_line(t_fdf *fdf, int row, int col, int scale, int offset_x, int offset_y)
{
    int x1, y1, z1, iso_x1, iso_y1;
    int x2, y2, z2, iso_x2, iso_y2;
    int color;

    x1 = col * scale;
    y1 = row * scale;
    z1 = fdf->map_data->points[row][col].z * scale / 10;
    iso_project(x1, y1, z1, &iso_x1, &iso_y1, offset_x, offset_y);

    x2 = (col + 1) * scale;
    y2 = row * scale;
    z2 = fdf->map_data->points[row][col + 1].z * scale / 10;
    iso_project(x2, y2, z2, &iso_x2, &iso_y2, offset_x, offset_y);

    color = 0xFFFFFF;
    if (z1 != 0 || z2 != 0)
        color = 0xFF0000;

    draw_line(fdf, iso_x1, iso_y1, iso_x2, iso_y2, color);
}

void draw_vertical_line(t_fdf *fdf, int row, int col, int scale, int offset_x, int offset_y)
{
    int x1, y1, z1, iso_x1, iso_y1;
    int x2, y2, z2, iso_x2, iso_y2;
    int color;

    x1 = col * scale;
    y1 = row * scale;
    z1 = fdf->map_data->points[row][col].z * scale / 10;
    iso_project(x1, y1, z1, &iso_x1, &iso_y1, offset_x, offset_y);

    x2 = col * scale;
    y2 = (row + 1) * scale;
    z2 = fdf->map_data->points[row + 1][col].z * scale / 10;
    iso_project(x2, y2, z2, &iso_x2, &iso_y2, offset_x, offset_y);

    color = 0xFFFFFF;
    if (z1 != 0 || z2 != 0)
        color = 0xFF0000;

    draw_line(fdf, iso_x1, iso_y1, iso_x2, iso_y2, color);
}

void render_data(t_fdf *fdf)
{
    int row;
    int col;
    int scale;
    int offset_x;
    int offset_y;

    row = 0;
    scale = 30;
    offset_x = WINDOW_WIDTH / 2;
    offset_y = WINDOW_HEIGHT / 4;

    while (row < fdf->map_data->max_m)
    {
        col = 0;
        while (col < fdf->map_data->max_n)
        {
            if (col < fdf->map_data->max_n - 1)
                draw_horizontal_line(fdf, row, col, scale, offset_x, offset_y);
            if (row < fdf->map_data->max_m - 1)
                draw_vertical_line(fdf, row, col, scale, offset_x, offset_y);
            col++;
        }
        row++;
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
	fdf->mlx_ptr = mlx_init();
	if (NULL == fdf->mlx_ptr)
	{
		free_points(fdf->map_data->points, fdf->map_data->max_m);
		free(fdf->map_data);
		return (-1);
	}
	fdf->win_ptr = mlx_new_window(fdf->mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, "TESTING");
	if (NULL == fdf->win_ptr)
	{
		mlx_destroy_display(fdf->mlx_ptr);
		free(fdf->mlx_ptr);
		free_points(fdf->map_data->points, fdf->map_data->max_m);
		free(fdf->map_data);
	}

	//draw_diagonal(fdf);
	render_data(fdf);
	mlx_loop(fdf->mlx_ptr);

	//print_map(fdf->map_data->points, fdf->map_data->max_n, fdf->map_data->max_m);
	// free options
	mlx_destroy_window(fdf->mlx_ptr,fdf->win_ptr);
	mlx_destroy_display(fdf->mlx_ptr);
	free(fdf->mlx_ptr);
	free_points(fdf->map_data->points, fdf->map_data->max_m);
	free(fdf->map_data);
	free(fdf);
}
