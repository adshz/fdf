/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 rotate.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/08/05 13:55:51 by szhong			   #+#	  #+#			  */
/*	 Updated: 2024/08/05 16:47:46 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "fdf.h"

static void	apply_rotation(float *a, float *b, double angle, t_rot_params p)
{
	float	tmp_a;
	float	tmp_b;

	tmp_a = *a * cos(angle) + p.sign1 * (*b) * sin(angle);
	tmp_b = p.sign2 * (*a) * sin(angle) + *b * cos(angle);
	*a = tmp_a;
	*b = tmp_b;
}

static void	rotate_axis(t_line *line, double angle, t_axis axis)
{
	t_rot_params	p_start;
	t_rot_params	p_end;

	if (axis == X_AXIS)
	{
		p_start = (t_rot_params){&line->start.y, &line->start.z, -1, 1};
		p_end = (t_rot_params){&line->end.y, &line->end.z, -1, 1};
	}
	else if (axis == Y_AXIS)
	{
		p_start = (t_rot_params){&line->start.x, &line->start.z, 1, -1};
		p_end = (t_rot_params){&line->end.x, &line->end.z, 1, -1};
	}
	else
	{
		p_start = (t_rot_params){&line->start.x, &line->start.z, -1, 1};
		p_end = (t_rot_params){&line->end.x, &line->end.z, -1, 1};
	}
	apply_rotation(p_start.a, p_start.b, angle, p_start);
	apply_rotation(p_end.a, p_end.b, angle, p_end);
}

void	x_rotation(t_cam *cam, t_line *line)
{
	if (cam->projection == PERSPECTIVE)
		rotate_axis(line, cam->alpha, X_AXIS);
}

void	rotate(t_cam *cam, t_line *line)
{
	rotate_axis(line, cam->alpha, X_AXIS);
	rotate_axis(line, cam->beta, Y_AXIS);
	rotate_axis(line, cam->gamma, Z_AXIS);
}
