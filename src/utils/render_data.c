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

static void	draw_thick_pixel(t_fdf *fdf, int x, int y, int colour)
{
	double		dx;
	double		dy;
	int			temp_x;
	int			temp_y;
	double		thickness;

	thickness = fdf->cam_ptr->line_thickness;
	dy = -thickness / 2;
	while (dy <= thickness / 2)
	{
		dx = -thickness / 2;
		while (dx <= thickness / 2)
		{
			temp_x = x + (int)dx;
			temp_y = y + (int)dy;
			if (temp_x >= 0 && temp_x < WINDOW_WIDTH \
					&& temp_y >= 0 && temp_y < WINDOW_HEIGHT)
				set_pixel_colour(fdf->img_ptr, temp_x, temp_y, colour);
			dx += 0.5;
		}
		dy += 0.5;
	}
}

static void	draw_params_init(t_cartesian *start, t_cartesian *end, \
		t_draw_params *p, char *slope)
{
	p->dx = end->x - start->x;
	p->dy = end->y - start->y;
	p->step = 1;
	if (ft_strcmp(slope, "low") == 0)
	{
		if (p->dy < 0)
		{
			p->step = -1;
			p->dy = -(p->dy);
		}
		p->diff = (2 * p->dy) - p->dx;
	}
	else
	{
		if (p->dx < 0)
		{
			p->step = -1;
			p->dx = -(p->dx);
		}
		p->diff = (2 * p->dx) - p->dy;
	}
	p->x = start->x;
	p->y = start->y;
}

static void	draw_line_low(t_fdf *fdf, t_cartesian start, t_cartesian end, \
		t_colour *color)
{
	t_draw_params	p;

	draw_params_init(&start, &end, &p, "low");
	while (p.x <= end.x)
	{
		p.draw_colour = get_colour(color, p.x - start.x, end.x - start.x);
		draw_thick_pixel(fdf, p.x, p.y, p.draw_colour);
		if (p.diff > 0)
		{
			p.y = p.y + p.step;
			p.diff = p.diff + (2 * (p.dy - p.dx));
		}
		else
			p.diff = p.diff + 2 * p.dy;
		p.x++;
	}
}

static void	draw_line_high(t_fdf *fdf, t_cartesian start, t_cartesian end, \
		t_colour *color)
{
	t_draw_params	p;

	draw_params_init(&start, &end, &p, "high");
	while (p.y <= end.y)
	{
		p.draw_colour = get_colour(color, p.y - start.y, end.y - start.y);
		draw_thick_pixel(fdf, p.x, p.y, p.draw_colour);
		if (p.diff > 0)
		{
			p.x = p.x + p.step;
			p.diff = p.diff + (2 * (p.dx - p.dy));
		}
		else
			p.diff = p.diff + 2 * p.dx;
		p.y++;
	}
}

static void	draw_line_bresenham(t_fdf *fdf, t_cartesian start, t_cartesian end)
{
	t_colour	*color;

	color = colour_init(start, end);
	if (!color)
		clean_free(fdf);
	if (fabsf(end.y - start.y) < fabsf(end.x - start.x))
	{
		if (start.x > end.x)
			draw_line_low(fdf, end, start, color);
		else
			draw_line_low(fdf, start, end, color);
	}
	else
	{
		if (start.y > end.y)
			draw_line_high(fdf, end, start, color);
		else
			draw_line_high(fdf, start, end, color);
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
//static void	draw_line_low(t_fdf *fdf, t_cartesian start, t_cartesian end, \
//		t_colour *color)
//{
//	t_draw_params	p;
//	int				yi;
//
//	p.dx = end.x - start.x;
//	p.dy = end.y - start.y;
//	yi = 1;
//	if (p.dy < 0)
//	{
//		yi = -1;
//		p.dy = -p.dy;
//	}
//	p.diff = (2 * p.dy) - p.dx;
//	p.x = start.x;
//	p.y = start.y;
//	while (p.x <= end.x)
//	{
//		p.draw_colour = get_colour(color, p.x - start.x, end.x - start.x);
//		draw_thick_pixel(fdf, p.x, p.y, p.draw_colour);
//		if (p.diff > 0)
//		{
//			p.y = p.y + yi;
//			p.diff = p.diff + (2 * (p.dy - p.dx));
//		}
//		else
//			p.diff = p.diff + 2 * p.dy;
//		p.x++;
//	}
//}
//
//static void	draw_line_high(t_fdf *fdf, t_cartesian start, t_cartesian end, \
//		t_colour *color)
//{
//	t_draw_params	p;
//	int				xi;
//
//	p.dx = end.x - start.x;
//	p.dy = end.y - start.y;
//	xi = 1;
//	if (p.dx < 0)
//	{
//		xi = -1;
//		p.dx = -p.dx;
//	}
//	p.diff = (2 * p.dx) - p.dy;
//	p.x = start.x;
//	p.y = start.y;
//	while (p.y <= end.y)
//	{
//		p.draw_colour = get_colour(color, p.y - start.y, end.y - start.y);
//		draw_thick_pixel(fdf, p.x, p.y, p.draw_colour);
//		if (p.diff > 0)
//		{
//			p.x = p.x + xi;
//			p.diff = p.diff + (2 * (p.dx - p.dy));
//		}
//		else
//			p.diff = p.diff + 2 * p.dx;
//		p.y++;
//	}
//}
