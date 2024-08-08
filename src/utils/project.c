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

typedef struct s_proj_params
{
	float	*x;
	float	*y;
	float	*z;
	float	transform_z;
}	t_proj_params;

static void	isometric(t_proj_params p)
{
	float	tmp_x;
	float	tmp_y;

	tmp_x = (*p.x - *p.y) * cos(ANG_30);
	tmp_y = (*p.x - *p.y) * sin(ANG_30) - *p.z;
	*p.x = tmp_x;
	*p.y = tmp_y;
}

static void	perspective(t_proj_params p)
{
	float	z;
	float	tmp_x;
	float	tmp_y;

	z = *p.z + p.transform_z;
	tmp_x = *p.x / z;
	tmp_y = *p.y / z;
	*p.x = tmp_x * p.transform_z;
	*p.y = -tmp_y * p.transform_z;
}

static void	project_point(t_cartesian *point, t_projection_type proj, float transform_z)
{
	t_proj_params	p;

	p.x = &point->x;
	p.y = &point->y;
	p.z = &point->z;
	p.transform_z = transform_z;
	if (proj == ISOMETRIC)
		isometric(p);
	else if (proj == PERSPECTIVE)
		perspective(p);
}

void	project(t_cam *cam, t_line *line)
{
	t_line	rotated_line;

	if (cam->projection == TOP)
		return ;
	rotated_line = *line;
	x_rotation(cam, &rotated_line);
	project_point(&(rotated_line.start), cam->projection, rotated_line.transform_z);
	project_point(&(rotated_line.end), cam->projection, rotated_line.transform_z);
	*line = rotated_line;
}
