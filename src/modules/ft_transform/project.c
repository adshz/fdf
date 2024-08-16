/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 project.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/08/05 14:47:23 by szhong			   #+#	  #+#			  */
/*	 Updated: 2024/08/05 16:57:51 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "fdf.h"

static void	isometric(t_proj_params p)
{
	float	tmp_x;
	float	tmp_y;
	float	stretch_factor;

	stretch_factor = 1;
	tmp_x = (*(p.x) - *(p.y)) * cos(ANG_30) * stretch_factor;
	tmp_y = (*(p.x) + *(p.y)) * sin(ANG_30) * -0.1 - *(p.z);
	*(p.x) = tmp_x;
	*(p.y) = tmp_y;
}

// transform_z controlled perspective effect z-offset projection
static void	perspective(t_proj_params p)
{
	float	z;
	float	x;
	float	y;
	float	scale;
	float	z_offset;

	z = *(p.z);
	x = *(p.x);
	y = *(p.y);
	z_offset = p.transform_z;
	if (fabsf(z + z_offset) < 0.001f)
		z = 0.001f - z_offset;
	scale = z_offset / (z + z_offset);
	x *= scale;
	y *= scale;
	*(p.x) = x;
	*(p.y) = y;
	*(p.z) = z;
}

static void	project_point(t_cartesian *point, t_proj_type proj, \
		float transform_z)
{
	t_proj_params	p;

	p.x = &(point->x);
	p.y = &(point->y);
	p.z = &(point->z);
	p.transform_z = transform_z;
	if (proj == ISOMETRIC)
		isometric(p);
	else if (proj == PERSPECTIVE)
		perspective(p);
}

void	project(t_cam *cam, t_line *line)
{
	t_line	rotated_line;

	rotated_line = *line;
	if (cam->projection != TOP)
		x_rotation(cam, &rotated_line);
	if (cam->projection == TOP)
	{
		rotated_line.start.y = rotated_line.start.z;
		rotated_line.end.y = rotated_line.end.z;
	}
	project_point(&(rotated_line.start), cam->projection, \
			rotated_line.transform_z);
	project_point(&(rotated_line.end), cam->projection, \
			rotated_line.transform_z);
	*line = rotated_line;
}
