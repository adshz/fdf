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

typedef struct s_transform_params
{
	float	*x;
	float	*y;
	float	scale_factor;
	float	cam_position_x;
	float	cam_position_y;
}	t_transform_params;

static void	apply_transform(t_transform_params *p, \
		void (*f)(t_transform_params *))
{
	f(p);
}

static void	scale_point(t_transform_params *p)
{
	*p->x *= p->scale_factor;
	*p->y *= p->scale_factor;
}

static void	translate_point(t_transform_params *p)
{
	*p->x += p->cam_position_x;
	*p->y += p->cam_position_y;
}

static void	transform_point(t_transform_params *p)
{
	apply_transform(p, scale_point);
	apply_transform(p, translate_point);
}

void	transform(t_cam *cam, t_line *line)
{
	t_transform_params	start;
	t_transform_params	end;


	start =(t_transform_params){&line->start.x, &line->start.y, \
		cam->scale_factor, cam->cam_position_x, cam->cam_position_y};
	end = (t_transform_params){&line->end.x, &line->end.y, \
		cam->scale_factor, cam->cam_position_x, cam->cam_position_y};
	transform_point(&start);
	transform_point(&end);
}

