/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:35:02 by szhong            #+#    #+#             */
/*   Updated: 2024/08/16 16:32:07 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fdf.h"

void	canvas_setup(t_img *img, size_t canvas_byte_size)
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

// convert colour value to the right position of 32-bit pixel 
// Calculate the pixel index in the buffer
// Handle big-edian case
// Handle little-edian case
// NOTES
//	to be honest, && 0xff is like a subtraction to keep the value you want 
//	delete the value you don't want
void	set_pixel_colour(t_img *img, float col, float row, int colour)
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

void	draw_thick_pixel(t_fdf *fdf, int x, int y, int colour)
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
