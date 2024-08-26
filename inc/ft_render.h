/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_render.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:11:54 by szhong            #+#    #+#             */
/*   Updated: 2024/08/21 22:32:38 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_RENDER_H
# define FT_RENDER_H

typedef struct s_draw_params
{
	int	dx;
	int	dy;
	int	diff;
	int	x;
	int	y;
	int	draw_colour;
	int	step;
}	t_draw_params;

typedef struct s_line_params
{
	int	x;
	int	y;
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	e2;
	int	max_steps;
	int	current_step;
}	t_line_params;

void	set_pixel_colour(t_img *img, float col, float row, int colour);
void	canvas_setup(t_img *img, size_t canvas_byte_size);
void	draw_low_slope(t_fdf *fdf, t_cartesian start, t_cartesian end, \
		t_colour *colour);
void	draw_high_slope(t_fdf *fdf, t_cartesian start, t_cartesian end, \
		t_colour *colour);
void	render_data(t_fdf *fdf);
void	draw_thick_pixel(t_fdf *fdf, int x, int y, int colour);
#endif
