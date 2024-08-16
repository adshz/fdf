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

// Normalize z value to [0, 1] range 
// factor = d(a point z - minimum z) / range_z value 
static int	set_point_colour(t_fdf *fdf, t_cartesian *point)
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
		point->colour = set_point_colour(fdf, point);
}
