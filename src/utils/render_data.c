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

static float	float_abs(float nbr)
{
	if (nbr < 0)
		return (nbr * (-1));
	return (nbr);
}

typedef struct s_line_params
{
    int x;
    int y;
    int dx;
    int dy;
    int sx;
    int sy;
    int err;
    int e2;
    int max_steps;
    int current_step;
} t_line_params;

// convert colour value to the right position of 32-bit pixel 
// Calculate the pixel index in the buffer
// Handle big-edian case
// Handle little-edian case
// NOTES
//	to be honest, && 0xff is like a subtraction to keep the value you want 
//	delete the value you don't want
static void	set_pixel_colour(t_img *img, float col, float row, int colour)
{
	int	pixel;

	pixel = ((int)row * img->line_len) + ((int)col * 4);
	if (img->endian == 1)
	{
		img->mem_addr[pixel + 0] = (colour >> 24);
		img->mem_addr[pixel + 1] = (colour >> 16) & 0xff;
		img->mem_addr[pixel + 2] = (colour >> 8) & 0xff;
		img->mem_addr[pixel + 3] = colour & 0xff;
	}
	else if (img->endian == 0)
	{
		img->mem_addr[pixel + 0] = colour & 0xff;
		img->mem_addr[pixel + 1] = (colour >> 8) & 0xff;
		img->mem_addr[pixel + 2] = (colour >> 16) & 0xff;
		img->mem_addr[pixel + 3] = (colour >> 24);
	}
	return ;
}

static void	draw_line_bresenham(t_fdf *fdf, t_cartesian start, t_cartesian end)
{
	float		dx;
	float		dy;
	int			max_steps;
	int			line_idx;
	t_colour	*colour;

	dx = abs((int)end.x - (int)start.x);
	dy = abs((int)end.y - (int)start.y);
	max_steps = (int)max(dx, dy);
	dx /= max_steps;
	dy /= max_steps;
	colour = colour_init(start, end);
	if (!colour)
		clean_free(fdf);
	line_idx = 0;
	while (line_idx < max_steps)
	{
		start.colour =  get_colour(colour, line_idx++, max_steps);
		if (start.x > 0 && start.y > 0 && start.x < WINDOW_WIDTH \
				&& start.y < WINDOW_HEIGHT)
			set_pixel_colour(fdf->img_ptr, start.x , start.y, start.colour);
		start.x += dx;
		start.y += dy;
	}
	free(colour);
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
	transform(fdf->cam_ptr, fdf->img_ptr->line_segment);
	draw_line_bresenham(fdf, fdf->img_ptr->line_segment->start, \
			fdf->img_ptr->line_segment->end);
	free(fdf->img_ptr->line_segment);
}

static void	canvas_setup(t_img *img, size_t canvas_byte_size)
{
	int	col;
	int	row;

	ft_bzero(img->mem_addr, canvas_byte_size);
	row = 0;
	while (row < WINDOW_HEIGHT)
	{
		col = 0;
		while (col < WINDOW_WIDTH)
		{
			set_pixel_colour(img, col, row, DEFAULT_BACKGROUND); 
			col++;
		}
		row++;
	}
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
			// horiztonal
			if (col < fdf->map_data->max_n - 1)
				draw_line_segment(fdf, fdf->map_data->points[row][col], \
						fdf->map_data->points[row][col + 1]);
			// vertical line
			if (row < fdf->map_data->max_m - 1)
				draw_line_segment(fdf, fdf->map_data->points[row][col], \
						fdf->map_data->points[row + 1][col]);
			col++;
		}
		row++;
	}
	mlx_put_image_to_window(fdf->mlx_ptr, fdf->win_ptr, fdf->img_ptr->img_buff, \
			0, 0);
}
