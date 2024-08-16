/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_slope.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:39:19 by szhong            #+#    #+#             */
/*   Updated: 2024/08/16 16:31:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fdf.h"

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

void	draw_low_slope(t_fdf *fdf, t_cartesian start, t_cartesian end, \
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

void	draw_high_slope(t_fdf *fdf, t_cartesian start, t_cartesian end, \
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
