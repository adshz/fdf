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

typedef enum e_axis
{
	X_AXIS,
	Y_AXIS,
	Z_AXIS
}	t_axis;

typedef struct s_rot_params
{
	float	*a;
	float	*b;
	int	sign1;
	int	sign2;
}	t_rot_params;

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

    const char *axis_name = (axis == X_AXIS) ? "X" : (axis == Y_AXIS) ? "Y" : "Z";

    printf("Rotating around %s-axis by angle %.2f\n", axis_name, angle);

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

	printf("Before rotation: Start(%.2f, %.2f, %.2f), End(%.2f, %.2f, %.2f)\n",
           line->start.x, line->start.y, line->start.z,
           line->end.x, line->end.y, line->end.z);

	apply_rotation(p_start.a, p_start.b, angle, p_start);
	apply_rotation(p_end.a, p_end.b, angle, p_end);
	printf("After rotation: Start(%.2f, %.2f, %.2f), End(%.2f, %.2f, %.2f)\n",
           line->start.x, line->start.y, line->start.z,
           line->end.x, line->end.y, line->end.z);
}

void	x_rotation(t_cam *cam, t_line *line)
{
	if (cam->projection == PERSPECTIVE)
		rotate_axis(line, cam->alpha, X_AXIS);
}

void	rotate(t_cam *cam, t_line *line)
{
	printf("Rotate - Before: Start(%.2f, %.2f, %.2f), End(%.2f, %.2f, %.2f)\n",
		line->start.x, line->start.y, line->start.z,
		line->end.x, line->end.y, line->end.z);

    printf("Rotating around X-axis (alpha: %.2f)\n", cam->alpha);
	rotate_axis(line, cam->alpha, X_AXIS);
	printf("After X rotation: Start(%.2f, %.2f, %.2f), End(%.2f, %.2f, %.2f)\n",
		line->start.x, line->start.y, line->start.z,
		line->end.x, line->end.y, line->end.z);


	printf("Rotating around Y-axis (beta: %.2f)\n", cam->beta);
	rotate_axis(line, cam->beta, Y_AXIS);
    printf("After Y rotation: Start(%.2f, %.2f, %.2f), End(%.2f, %.2f, %.2f)\n",
           line->start.x, line->start.y, line->start.z,
           line->end.x, line->end.y, line->end.z);

	printf("Rotating around Z-axis (gamma: %.2f)\n", cam->gamma);
	rotate_axis(line, cam->gamma, Z_AXIS);
	printf("After Z rotation: Start(%.2f, %.2f, %.2f), End(%.2f, %.2f, %.2f)\n",
           line->start.x, line->start.y, line->start.z,
           line->end.x, line->end.y, line->end.z);
}
