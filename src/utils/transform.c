/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 transform.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/08/05 16:27:49 by szhong			   #+#	  #+#			  */
/*	 Updated: 2024/08/05 16:37:21 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "fdf.h"
#include <math.h>

void	scale(t_line *line, int scale_factor)
{
	line->start.x *= scale_factor;
	line->start.y *= scale_factor;
	line->end.x *= scale_factor;
	line->end.y *= scale_factor;
}

void	translate(t_line *line, int cam_position_x, int cam_position_y)
{
	line->start.x += cam_position_x;
	line->start.y += cam_position_y;
	line->end.x += cam_position_x;
	line->end.y += cam_position_y;
}

void	flip_y_axis(t_line *line)
{
	line->start.y = WINDOW_HEIGHT - line->start.y;
	line->end.y = WINDOW_HEIGHT - line->end.y;
}

void	view_transform(t_cam *cam, t_line *line)
{
	scale(line, cam->scale_factor);
	if (cam->projection == TOP)
		translate(line, cam->cam_position_x, cam->cam_position_y);
	else
	{
		translate(line, cam->cam_position_x, cam->cam_position_y);
		flip_y_axis(line);
	}
}
