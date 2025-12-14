/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:35:02 by szhong            #+#    #+#             */
/*   Updated: 2024/08/21 22:33:49 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fdf.h"

// Clear the canvas with background color
void	canvas_setup(t_img *img, size_t canvas_byte_size)
{
	mlx_image_t	*mlx_img;

	(void)canvas_byte_size;
	mlx_img = (mlx_image_t *)img->img;
	ft_memset(mlx_img->pixels, 0, mlx_img->width * mlx_img->height * 4);
}

// Convert RGB color to RGBA format for MLX42
// MLX42 expects colors in RGBA format: 0xRRGGBBAA
static uint32_t	rgb_to_rgba(int colour)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	r = (colour >> 16) & 0xFF;
	g = (colour >> 8) & 0xFF;
	b = colour & 0xFF;
	return ((r << 24) | (g << 16) | (b << 8) | 0xFF);
}

// Set pixel color using MLX42's mlx_put_pixel
void	set_pixel_colour(t_img *img, float col, float row, int colour)
{
	mlx_image_t	*mlx_img;
	int			x;
	int			y;

	x = (int)col;
	y = (int)row;
	if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
		return ;
	mlx_img = (mlx_image_t *)img->img;
	mlx_put_pixel(mlx_img, x, y, rgb_to_rgba(colour));
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
