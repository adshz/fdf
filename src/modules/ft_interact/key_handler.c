/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:37:19 by szhong            #+#    #+#             */
/*   Updated: 2024/08/01 18:03:12 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fdf.h"

static void	key_thickness(keys_t keycode, t_fdf *fdf)
{
	if (keycode == MLX_KEY_EQUAL)
		fdf->cam_ptr->line_thickness += 0.5;
	else if (keycode == MLX_KEY_MINUS)
		fdf->cam_ptr->line_thickness -= 0.5;
	if (fdf->cam_ptr->line_thickness <= 0.2)
		fdf->cam_ptr->line_thickness = 0.5;
	else if (fdf->cam_ptr->line_thickness > 10)
		fdf->cam_ptr->line_thickness = 10;
}

static void	reset(t_fdf *fdf)
{
	fdf->cam_ptr->scale_factor = scale_to_fit(fdf->map_data);
	fdf->cam_ptr->scale_z = -0.16;
	fdf->cam_ptr->cam_position_x = WINDOW_WIDTH / 2;
	fdf->cam_ptr->cam_position_y = WINDOW_HEIGHT / 2;
	fdf->cam_ptr->alpha = ANG_30;
	fdf->cam_ptr->beta = 0.0;
	fdf->cam_ptr->gamma = ANG_45;
	fdf->cam_ptr->line_thickness = 1.5;
	fdf->cam_ptr->projection = ISOMETRIC;
	fdf->cam_ptr->colour_pallet = true;
}

// MLX42 key callback - called on key press/release/repeat
void	key_handler(mlx_key_data_t keydata, void *param)
{
	t_fdf	*fdf;
	keys_t	key;

	fdf = (t_fdf *)param;
	key = keydata.key;
	if (keydata.action != MLX_PRESS && keydata.action != MLX_REPEAT)
		return ;
	if (key == MLX_KEY_ESCAPE)
		mlx_close_window(fdf->mlx);
	else if (key == MLX_KEY_LEFT || key == MLX_KEY_RIGHT
		|| key == MLX_KEY_UP || key == MLX_KEY_DOWN)
		key_rotate(key, fdf);
	else if (key == MLX_KEY_EQUAL || key == MLX_KEY_MINUS)
		key_thickness(key, fdf);
	else if (key == MLX_KEY_PAGE_UP || key == MLX_KEY_PAGE_DOWN)
		key_scale(key, fdf);
	else if (key == MLX_KEY_SPACE)
		key_change_colour(key, fdf);
	else if (key == MLX_KEY_W || key == MLX_KEY_S
		|| key == MLX_KEY_D || key == MLX_KEY_A)
		key_translate(key, fdf);
	else if (key == MLX_KEY_I || key == MLX_KEY_P || key == MLX_KEY_T)
		key_projection(key, fdf);
	else if (key == MLX_KEY_R)
		reset(fdf);
	render_data(fdf);
}
