/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 render_data.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/08/01 16:33:37 by szhong			   #+#	  #+#			  */
/*	 Updated: 2024/08/05 17:04:27 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "fdf.h"

static void	draw_line_bresenham(t_fdf *fdf, t_cartesian start, t_cartesian end)
{
	t_colour	*color;

	color = colour_init(start, end);
	if (!color)
		clean_free(fdf);
	if (fabsf(end.y - start.y) < fabsf(end.x - start.x))
	{
		if (start.x > end.x)
			draw_low_slope(fdf, end, start, color);
		else
			draw_low_slope(fdf, start, end, color);
	}
	else
	{
		if (start.y > end.y)
			draw_high_slope(fdf, end, start, color);
		else
			draw_high_slope(fdf, start, end, color);
	}
	free(color);
}

static void	draw_line_segment(t_fdf *fdf, t_cartesian start, t_cartesian end)
{
	if (!fdf || !fdf->cam_ptr || !fdf->img_ptr)
		return ;
	start.z *= fdf->cam_ptr->scale_z;
	end.z *= fdf->cam_ptr->scale_z;
	apply_colours(fdf, &start);
	apply_colours(fdf, &end);
	fdf->img_ptr->line_segment = line_init(start, end, fdf);
	if (!fdf->img_ptr->line_segment)
		clean_free(fdf);
	rotate(fdf->cam_ptr, fdf->img_ptr->line_segment);
	project(fdf->cam_ptr, fdf->img_ptr->line_segment);
	view_transform(fdf->cam_ptr, fdf->img_ptr->line_segment);
	draw_line_bresenham(fdf, fdf->img_ptr->line_segment->start, \
			fdf->img_ptr->line_segment->end);
	free(fdf->img_ptr->line_segment);
}

void	render_data(t_fdf *fdf)
{
	int	row;
	int	col;

	row = 0;
	canvas_setup(fdf->img_ptr, PIXEL_COUNTS * 4);
	while (row < fdf->map_data->max_m)
	{
		col = 0;
		while (col < fdf->map_data->max_n)
		{
			if (col < fdf->map_data->max_n - 1)
				draw_line_segment(fdf, fdf->map_data->points[row][col], \
						fdf->map_data->points[row][col + 1]);
			if (row < fdf->map_data->max_m - 1)
				draw_line_segment(fdf, fdf->map_data->points[row][col], \
						fdf->map_data->points[row + 1][col]);
			col++;
		}
		row++;
	}
	mlx_put_image_to_window(fdf->mlx_ptr, fdf->win_ptr, fdf->img_ptr->img_buff, \
			0, 0);
	print_menu(fdf);
}
