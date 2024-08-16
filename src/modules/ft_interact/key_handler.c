/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 key_handler.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/08/01 15:37:19 by szhong			   #+#	  #+#			  */
/*	 Updated: 2024/08/01 18:03:12 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "fdf.h"

static void	key_thickness(int keycode, t_fdf *fdf)
{
	if (keycode == 61)
		fdf->cam_ptr->line_thickness += 0.5;
	else if (keycode == XK_minus)
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

int	key_handler(int keycode, t_fdf *fdf)
{
	if (keycode == XK_Escape)
		esc_close(keycode, fdf);
	else if (keycode == XK_Left || keycode == XK_Right || keycode == XK_Up \
			|| keycode == XK_Down)
		key_rotate(keycode, fdf);
	else if (keycode == 61 || keycode == XK_minus)
		key_thickness(keycode, fdf);
	else if (keycode == XK_Page_Up || keycode == XK_Page_Down)
		key_scale(keycode, fdf);
	else if (keycode == XK_space)
		key_change_colour(keycode, fdf);
	else if (keycode == XK_W || keycode == 'w' || keycode == 'W' \
			|| keycode == XK_S || keycode == 's' || keycode == 'S' \
			|| keycode == XK_D || keycode == 'd' || keycode == 'D' \
			|| keycode == XK_A || keycode == 'a' || keycode == 'A')
		key_translate(keycode, fdf);
	else if (keycode == XK_I || keycode == 'i' || keycode == 'I' \
			|| keycode == XK_P || keycode == 'p' || keycode == 'P' \
			|| keycode == XK_T || keycode == 't' || keycode == 'T')
		key_projection(keycode, fdf);
	else if (keycode == XK_R || keycode == 'r' || keycode == 'R')
		reset(fdf);
	render_data(fdf);
	return (0);
}
