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

int	key_handler(int keycode, t_fdf *fdf)
{
	if (keycode == XK_Escape)
		esc_close(keycode, fdf);
	else if (keycode == XK_Left || keycode == XK_Right || keycode == XK_Up \
			|| keycode == XK_Down)
		key_rotate(keycode, fdf);
	else if (keycode == 61 || keycode == XK_minus)
		key_thickness(keycode, fdf);
	render_data(fdf);
	return (0);
//	mlx_hook(fdf->win_ptr, 17, 1L<<17, esc_close, &fdf);
//	mlx_hook(fdf->win_ptr, 2, 1L<<19, click_close, &fdf);
//	mlx_hook(fdf->win_ptr, 2, 1L<<0, rotate_keypress, &fdf);
}

//int	mouse_hanlder(int mousecode, t_fdf *fdf)
//{
//	if (mousecode == 
//}
