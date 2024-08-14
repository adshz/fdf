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
#include <X11/keysym.h>
#include <X11/X.h>

# define ROTATION_STEP 0.1

static void	win_clean_free(t_fdf *fdf)
{
	if (fdf)
	{
		if (fdf->mlx_ptr)
		{
			if (fdf->img_ptr && fdf->img_ptr->img_buff)
			{
				mlx_destroy_image(fdf->mlx_ptr, fdf->img_ptr->img_buff);
				fdf->img_ptr->img_buff = NULL;
			}
			mlx_destroy_window(fdf->mlx_ptr, fdf->win_ptr);
			mlx_destroy_display(fdf->mlx_ptr);
			free(fdf->mlx_ptr);
		}
		free(fdf->cam_ptr);
		free(fdf->img_ptr);
		if (fdf->map_data)
		{
			free_points(fdf->map_data->points, fdf->map_data->max_m);
			free(fdf->map_data);
		}
		free(fdf);
	}
	exit(1);
}

void	esc_close(int keycode, t_fdf *fdf)
{
	if (keycode == XK_Escape)
	{
		win_clean_free(fdf);
		exit(0);
	}
	return ;
}

int	close_window(t_fdf *fdf)
{
	win_clean_free(fdf);
	exit(0);
	return (0);
}

static void	key_rotate(int keycode, t_fdf *fdf)
{
	if (keycode == XK_Left)
		fdf->cam_ptr->beta -= ROTATION_STEP;
	else if (keycode == XK_Right)
		fdf->cam_ptr->beta += ROTATION_STEP;
	else if (keycode == XK_Up)
		fdf->cam_ptr->alpha -= ROTATION_STEP;
	else if (keycode == XK_Down)
		fdf->cam_ptr->alpha += ROTATION_STEP;
}

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

static void key_scale(int keycode, t_fdf *fdf)
{
	float	scale_step;

	scale_step = 1.1f;
	if (keycode == XK_Page_Up)
		fdf->cam_ptr->scale_factor *= scale_step;
	else if (keycode == XK_Page_Down)
		fdf->cam_ptr->scale_factor /= scale_step;
}

static void reset(t_fdf *fdf)
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

static void key_change_colour(int keycode, t_fdf *fdf)
{
	if (keycode == XK_space)
	{
		if (fdf->cam_ptr->colour_pallet == true)
			fdf->cam_ptr->colour_pallet = false;
		else
			fdf->cam_ptr->colour_pallet = true;
	}
}

static void key_translate(int keycode, t_fdf *fdf)
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

static void key_projection(int keycode, t_fdf *fdf)
{
	if (keycode == XK_I || keycode == 'i' || keycode == 'I')
		fdf->cam_ptr->projection = ISOMETRIC;
	else if (keycode == XK_P || keycode == 'p' || keycode == 'P')
		fdf->cam_ptr->projection = PERSPECTIVE;
	else if (keycode == XK_T || keycode == 't' || keycode == 'T')
		fdf->cam_ptr->projection = TOP;
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
//	mlx_hook(fdf->win_ptr, 17, 1L<<17, esc_close, &fdf);
//	mlx_hook(fdf->win_ptr, 2, 1L<<19, click_close, &fdf);
//	mlx_hook(fdf->win_ptr, 2, 1L<<0, rotate_keypress, &fdf);
}

//int	mouse_handler(int mousecode, t_fdf *fdf)
//{
//	float	scale_step;
//
//	scale_step = 0.1f;
//	if (mousecode == 1)
//		fdf->cam_ptr->scale_factor += scale_step;
//	else if (mousecode == 2)
//		fdf->cam_ptr->scale_factor -= scale_step;
//	if (fdf->cam_ptr->scale_factor < 0.1f)
//		fdf->cam_ptr->scale_factor = 0.1f;
//	else if (fdf->cam_ptr->scale_factor > 10.0f)
//		fdf->cam_ptr->scale_factor = 10.0f;
//	render_data(fdf);
//	return (0);
//}
