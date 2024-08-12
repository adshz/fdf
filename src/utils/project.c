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
#include <float.h>

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
	float	stretch_factor;
	
	stretch_factor = 1;
	tmp_x = (*(p.x) - *(p.y)) * cos(ANG_30) * stretch_factor;
	tmp_y = (*(p.x) + *(p.y)) * sin(ANG_30) *-0.1 - *(p.z);
	*(p.x) = tmp_x;
	*(p.y) = tmp_y;
}

static void	perspective(t_proj_params p)
{
	float	z;
	float	tmp_x;
	float	tmp_y;

	z = *(p.z) + p.transform_z;
	tmp_x = *(p.x) / z;
	tmp_y = *(p.y) / z;
	*(p.x) = tmp_x * p.transform_z;
	*(p.y) = tmp_y * p.transform_z;
}

static void	project_point(t_cartesian *point, t_projection_type proj, float transform_z)
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

	if (cam->projection == TOP)
		return ;
	rotated_line = *line;
	printf("Project - Before rotation: Start(%.2f, %.2f, %.2f), End(%.2f, %.2f, %.2f)\n",
		rotated_line.start.x, rotated_line.start.y, rotated_line.start.z,
		rotated_line.end.x, rotated_line.end.y, rotated_line.end.z);

	
	x_rotation(cam, &rotated_line);

	printf("Project - After rotation: Start(%.2f, %.2f, %.2f), End(%.2f, %.2f, %.2f)\n",
		rotated_line.start.x, rotated_line.start.y, rotated_line.start.z,
		rotated_line.end.x, rotated_line.end.y, rotated_line.end.z);


	project_point(&(rotated_line.start), cam->projection, rotated_line.transform_z);
	project_point(&(rotated_line.end), cam->projection, rotated_line.transform_z);
	

	printf("Project - After projection: Start(%.2f, %.2f, %.2f), End(%.2f, %.2f, %.2f)\n",
		rotated_line.start.x, rotated_line.start.y, rotated_line.start.z,
		rotated_line.end.x, rotated_line.end.y, rotated_line.end.z);
	*line = rotated_line;

}
