/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colour_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 15:30:07 by szhong            #+#    #+#             */
/*   Updated: 2024/08/21 21:58:19 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_colour.h"

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

	if (line_size == 0 || i_line < 0 || i_line > line_size)
		return (colour->start_colour);
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
