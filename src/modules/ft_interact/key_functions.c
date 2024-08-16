/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 15:54:28 by szhong            #+#    #+#             */
/*   Updated: 2024/08/16 16:29:20 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fdf.h"

void	key_rotate(int keycode, t_fdf *fdf)
{
	if (keycode == XK_Left)
		fdf->cam_ptr->beta -= 0.1;
	else if (keycode == XK_Right)
		fdf->cam_ptr->beta += 0.1;
	else if (keycode == XK_Up)
		fdf->cam_ptr->alpha -= 0.1;
	else if (keycode == XK_Down)
		fdf->cam_ptr->alpha += 0.1;
}

void	key_scale(int keycode, t_fdf *fdf)
{
	float	scale_step;

	scale_step = 1.1f;
	if (keycode == XK_Page_Up)
		fdf->cam_ptr->scale_factor *= scale_step;
	else if (keycode == XK_Page_Down)
		fdf->cam_ptr->scale_factor /= scale_step;
}

void	key_translate(int keycode, t_fdf *fdf)
{
	if (keycode == XK_W || keycode == 'w' || keycode == 'W')
		fdf->cam_ptr->cam_position_y += 10;
	else if (keycode == XK_S || keycode == 's' || keycode == 'S')
		fdf->cam_ptr->cam_position_y -= 10;
	else if (keycode == XK_D || keycode == 'd' || keycode == 'D')
		fdf->cam_ptr->cam_position_x += 10;
	else if (keycode == XK_A || keycode == 'a' || keycode == 'A')
		fdf->cam_ptr->cam_position_x -= 10;
}

void	key_projection(int keycode, t_fdf *fdf)
{
	if (keycode == XK_I || keycode == 'i' || keycode == 'I')
		fdf->cam_ptr->projection = ISOMETRIC;
	else if (keycode == XK_P || keycode == 'p' || keycode == 'P')
		fdf->cam_ptr->projection = PERSPECTIVE;
	else if (keycode == XK_T || keycode == 't' || keycode == 'T')
		fdf->cam_ptr->projection = TOP;
}

void	key_change_colour(int keycode, t_fdf *fdf)
{
	if (keycode == XK_space)
	{
		if (fdf->cam_ptr->colour_pallet == true)
			fdf->cam_ptr->colour_pallet = false;
		else
			fdf->cam_ptr->colour_pallet = true;
	}
}
