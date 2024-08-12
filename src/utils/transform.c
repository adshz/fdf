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
#include <float.h>

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
	translate(line, cam->cam_position_x, cam->cam_position_y);
	flip_y_axis(line);
}
// typedef struct s_transform_params
// {
// 	float	*x;
// 	float	*y;
// 	float	scale_factor;
// 	float	cam_position_x;
// 	float	cam_position_y;
// }	t_transform_params;

// static void	apply_transform(t_transform_params *p, \
// 		void (*f)(t_transform_params *))
// {
// 	f(p);
// }

// static void	scale_point(t_transform_params *p)
// {
// 	*p->x *= p->scale_factor;
// 	*p->y *= p->scale_factor;
// }

// static void	translate_point(t_transform_params *p)
// {
// 	*p->x += p->cam_position_x;
// 	*p->y += p->cam_position_y;
// }

// static void transform_point(t_transform_params *p)
// {
//     // Scale the point
//     float scaled_x = *p->x * p->scale_factor;
//     float scaled_y = *p->y * p->scale_factor;
    
//     // Center and translate
//     scaled_x += p->cam_position_x;
//     scaled_y += p->cam_position_y;

//     // Ensure points stay within window bounds
//     scaled_x = fmaxf(0, fminf(scaled_x, WINDOW_WIDTH - 1));
//     scaled_y = fmaxf(0, fminf(scaled_y, WINDOW_HEIGHT - 1));

//     // Convert back to int
//     *p->x = (int)roundf(scaled_x);
//     *p->y = (int)roundf(scaled_y);
// }

// // static void transform_point(t_transform_params *p)
// // {
// // 	float	scaled_x;
// // 	float	scaled_y;

// //     // First, scale the point
// //     scaled_x = *p->x * p->scale_factor;
// //     scaled_y = *p->y * p->scale_factor;
    
// //     // Then, center the coordinate system
// //     scaled_x += p->cam_position_x;
// //     scaled_y += p->cam_position_y;
    
// //     // Ensure the point is within the window boundaries
// // 	*p->x = (int )roundf(scaled_x);
// //     *p->y = (int )roundf(scaled_y);
// // }
// // static void	transform_point(t_transform_params *p)
// // {
// // 	printf("Before scale: (%.2f, %.2f)\n", *p->x, *p->y);
// //     apply_transform(p, scale_point);
// //     printf("After scale: (%.2f, %.2f)\n", *p->x, *p->y);
// //     apply_transform(p, translate_point);
// //     printf("After translate: (%.2f, %.2f)\n", *p->x, *p->y);
// // }


// void	transform(t_cam *cam, t_line *line)
// {
// 	t_transform_params	start;
// 	t_transform_params	end;


// 	printf("Before viewport transform: Start(%.2f, %.2f), End(%.2f, %.2f)\n",
//            line->start.x, line->start.y, line->end.x, line->end.y);
//     printf("Camera position: (%.2f, %.2f)\n", cam->cam_position_x, cam->cam_position_y);
//     printf("Scale factor: %.2f\n", cam->scale_factor);


// 	start =(t_transform_params){&line->start.x, &line->start.y, \
// 		cam->scale_factor, cam->cam_position_x, cam->cam_position_y};
// 	end = (t_transform_params){&line->end.x, &line->end.y, \
// 		cam->scale_factor, cam->cam_position_x, cam->cam_position_y};

// 	printf("After scaling - Start(%.2f, %.2f), End(%.2f, %.2f)\n",
//            *start.x * start.scale_factor, *start.y * start.scale_factor,
//            *end.x * end.scale_factor, *end.y * end.scale_factor);

// 	transform_point(&start);
// 	transform_point(&end);

// 	printf("After viewport transform: Start(%.2f, %.2f), End(%.2f, %.2f)\n",
//            line->start.x, line->start.y, line->end.x, line->end.y);
// }

