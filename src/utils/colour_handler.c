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
# define LOW_COLOUR 0x008080   // Teal
# define MID_COLOUR 0xFFA500   // Orange
# define HIGH_COLOUR 0xFF00FF  // Magenta
							   //
t_colour	*colour_init(t_cartesian start, t_cartesian end)
{
	t_colour	*colour;

	colour = malloc(sizeof(t_colour));
	if (!colour)
		return (NULL);
	colour->start_colour = start.colour & 0xFFFFFF;
	colour->end_colour = end.colour & 0xFFFFFF;
	colour->delta_r = ((end.colour >> 16) & 0xFF) - ((start.colour >> 16) & 0xFF);
	colour->delta_g = ((end.colour >> 8) & 0xFF) - ((start.colour >> 8) & 0xFF);
	colour->delta_b = (end.colour & 0xFF) - (start.colour & 0xFF);
	return (colour);
}

int ft_clamp(int value, int min, int max)
{
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

int get_colour(t_colour *colour, int i_line, int line_size)
{
	int r;
	int g;
	int b;
	float progress;

	progress = (float)i_line / line_size;
	r = ((colour->start_colour >> 16) & 0xFF) + (int)(colour->delta_r * progress);
	g = ((colour->start_colour >> 8) & 0xFF) + (int)(colour->delta_g * progress);
	b = (colour->start_colour & 0xFF) + (int)(colour->delta_b * progress);
	r = ft_clamp(r, 0, 255);
	g = ft_clamp(g, 0, 255);
	b = ft_clamp(b, 0, 255);
	return (r << 16) | (g << 8) | b;
}

static int	interpolate_component(int start, int end, float factor)
{
	int	result;

	result = start * (1 - factor) + end * factor;
	return (result);
}

static int	interpolate_colour(int colour_a, int colour_b, float factor)
{
	int	r;
	int	g;
	int	b;

	r = interpolate_component((colour_a >> 16) & 0xFF, (colour_b >> 16) & 0xFF, factor);
	g = interpolate_component((colour_a >> 8) & 0xFF, (colour_b >> 8) & 0xFF, factor);
	b = interpolate_component((colour_a) & 0xFF, (colour_b) & 0xFF, factor);

	return ((r << 16) | (g << 8) | b);
}

static int	calculate_colour(float factor)
{
	int	colour;

	if (factor < 0.5)
		colour = interpolate_colour(LOW_COLOUR, MID_COLOUR, factor * 2);
	else
		colour = interpolate_colour(MID_COLOUR, HIGH_COLOUR, (factor - 0.5) * 2);
	return (colour);
}

//static	t_colour	*colour_pallet_init(int min_colour, int max_colour)
//{
//	t_colour	*colour;
//
//	colour = malloc(sizeof(t_colour));
//	if (!colour)
//		return (NULL);
//	colour->start_colour = min_colour;
//	colour->start_r = (RED & min_colour) >> 16;
//	colour->start_g = (GREEN & min_colour) >> 8;
//	colour->start_b = (BLUE & min_colour);
//	colour->end_colour = max_colour;
//	colour->end_r = (RED & max_colour) >> 16;
//	colour->end_g = (GREEN & max_colour) >> 8;
//	colour->end_b = (BLUE & max_colour);
//	colour->delta_r = (colour->end_r - colour->start_r);
//	colour->delta_g = (colour->end_g - colour->start_g);
//	colour->delta_b = (colour->end_b - colour->start_b);
//	return (colour);
//}

static int	get_point_colour(t_fdf *fdf, t_cartesian *point)
{
	float	factor;
	int		value_colour;
	t_map	*map_file;

	map_file = fdf->map_data;
	if (map_file->max_z == map_file->min_z)
		return (MID_COLOUR);
	factor = (float )(point->z - map_file->min_z) / (map_file->max_z - map_file->min_z);
	value_colour = calculate_colour(factor);
	return (value_colour);
}

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
