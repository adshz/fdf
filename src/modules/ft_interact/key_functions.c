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

void	key_rotate(keys_t keycode, t_fdf *fdf)
{
	if (keycode == MLX_KEY_LEFT)
		fdf->cam_ptr->beta -= 0.1;
	else if (keycode == MLX_KEY_RIGHT)
		fdf->cam_ptr->beta += 0.1;
	else if (keycode == MLX_KEY_UP)
		fdf->cam_ptr->alpha -= 0.1;
	else if (keycode == MLX_KEY_DOWN)
		fdf->cam_ptr->alpha += 0.1;
}

void	key_scale(keys_t keycode, t_fdf *fdf)
{
	float	scale_step;

	scale_step = 1.1f;
	if (keycode == MLX_KEY_PAGE_UP)
		fdf->cam_ptr->scale_factor *= scale_step;
	else if (keycode == MLX_KEY_PAGE_DOWN)
		fdf->cam_ptr->scale_factor /= scale_step;
}

void	key_translate(keys_t keycode, t_fdf *fdf)
{
	if (keycode == MLX_KEY_W)
		fdf->cam_ptr->cam_position_y += 10;
	else if (keycode == MLX_KEY_S)
		fdf->cam_ptr->cam_position_y -= 10;
	else if (keycode == MLX_KEY_D)
		fdf->cam_ptr->cam_position_x += 10;
	else if (keycode == MLX_KEY_A)
		fdf->cam_ptr->cam_position_x -= 10;
}

void	key_projection(keys_t keycode, t_fdf *fdf)
{
	if (keycode == MLX_KEY_I)
		fdf->cam_ptr->projection = ISOMETRIC;
	else if (keycode == MLX_KEY_P)
		fdf->cam_ptr->projection = PERSPECTIVE;
	else if (keycode == MLX_KEY_T)
		fdf->cam_ptr->projection = TOP;
}

void	key_change_colour(keys_t keycode, t_fdf *fdf)
{
	if (keycode == MLX_KEY_SPACE)
	{
		if (fdf->cam_ptr->colour_pallet == true)
			fdf->cam_ptr->colour_pallet = false;
		else
			fdf->cam_ptr->colour_pallet = true;
	}
}
