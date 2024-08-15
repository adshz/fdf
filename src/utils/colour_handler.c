/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 colour_hanlder.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/08/05 17:04:50 by szhong			   #+#	  #+#			  */
/*	 Updated: 2024/08/05 17:27:19 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "fdf.h"

static void	initial_p_colour(float factor, t_col_params *p)
{
	if (factor < 0.5)
	{
		(*p).start_colour = LOW_COLOUR;
		(*p).end_colour = MID_COLOUR;
		factor = factor * 2;
	}
	else
	{
		(*p).start_colour = MID_COLOUR;
		(*p).end_colour = HIGH_COLOUR;
		factor = (factor - 0.5) * 2;
	}
	return ;
}

static int	calculate_gradient_colour(float factor)
{
	t_col_params	p;

	initial_p_colour(factor, &p);
	p.start_r = (p.start_colour >> 16) & 0xFF;
	p.start_g = (p.start_colour >> 8) & 0xFF;
	p.start_b = p.start_colour & 0xFF;
	p.end_r = (p.end_colour >> 16) & 0xFF;
	p.end_g = (p.end_colour >> 8) & 0xFF;
	p.end_b = p.end_colour & 0xFF;
	p.r = p.start_r + (int)((p.end_r - p.start_r) * factor);
	p.g = p.start_g + (int)((p.end_g - p.start_g) * factor);
	p.b = p.start_b + (int)((p.end_b - p.start_b) * factor);
	return ((p.r << 16) | (p.g << 8) | p.b);
}

t_colour	*colour_init(t_cartesian start, t_cartesian end)
{
	t_colour	*colour;
	int			e_col;
	int			s_col;

	s_col = start.colour;
	e_col = end.colour;
	colour = malloc(sizeof(t_colour));
	if (!colour)
		return (NULL);
	colour->start_colour = s_col & 0xFFFFFF;
	colour->end_colour = e_col & 0xFFFFFF;
	colour->delta_r = ((e_col >> 16) & 0xFF) - ((s_col >> 16) & 0xFF);
	colour->delta_g = ((e_col >> 8) & 0xFF) - ((s_col >> 8) & 0xFF);
	colour->delta_b = (e_col & 0xFF) - (s_col & 0xFF);
	return (colour);
}

int	ft_clamp(int value, int min, int max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

int	get_colour(t_colour *colour, int i_line, int line_size)
{
	float	progress;
	int		r;
	int		g;
	int		b;
	int		c_s_col;

	progress = (float)i_line / line_size;
	c_s_col = colour->start_colour;
	r = ((c_s_col >> 16) & 0xFF) + (int)(colour->delta_r * progress);
	g = ((c_s_col >> 8) & 0xFF) + (int)(colour->delta_g * progress);
	b = (c_s_col & 0xFF) + (int)(colour->delta_b * progress);
	r = ft_clamp(r, 0, 255);
	g = ft_clamp(g, 0, 255);
	b = ft_clamp(b, 0, 255);
	return ((r << 16) | (g << 8) | b);
}

static int	interpolate_component(int start, int end, float factor)
{
	int	result;

	result = start * (1 - factor) + end * factor;
	return (result);
}

static int	interpolate_colour(int c_a, int c_b, float factor)
{
	int	r;
	int	g;
	int	b;

	r = interpolate_component((c_a >> 16) & 0xFF, (c_b >> 16) & 0xFF, factor);
	g = interpolate_component((c_a >> 8) & 0xFF, (c_b >> 8) & 0xFF, factor);
	b = interpolate_component((c_a) & 0xFF, (c_b) & 0xFF, factor);
	return ((r << 16) | (g << 8) | b);
}

static int	calculate_colour(float factor)
{
	int	colour;
	int	adj_fac;

	if (factor < 0.5)
	{
		adj_fac = factor * 2;
		colour = interpolate_colour(LOW_COLOUR, MID_COLOUR, adj_fac);
	}
	else
	{
		adj_fac = (factor - 0.5) * 2;
		colour = interpolate_colour(MID_COLOUR, HIGH_COLOUR, adj_fac);
	}
	return (colour);
}

// Normalize z value to [0, 1] range 
// factor = d(a point z - minimum z) / range_z value 
static int	get_point_colour(t_fdf *fdf, t_cartesian *point)
{
	float	factor;
	int		value_colour;
	t_map	*map_file;

	map_file = fdf->map_data;
	if (map_file->max_z == map_file->min_z)
		return (MID_COLOUR);
	factor = (point->z - map_file->min_z) / (map_file->max_z - map_file->min_z);
	value_colour = calculate_gradient_colour(factor);
	value_colour = calculate_gradient_colour(factor);
	return (value_colour);
}

//	if point->colour != -1 then  Keeping original colour: 0x%X\n"
void	apply_colours(t_fdf *fdf, t_cartesian *point)
{
	if (fdf->cam_ptr->colour_pallet == false)
	{
		if (point->colour == -1)
			point->colour = DEFAULT_COLOUR;
	}
	else
		point->colour = get_point_colour(fdf, point);
}
