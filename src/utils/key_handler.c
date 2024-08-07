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

int	esc_close(int	keycode, t_fdf *fdf)
{
	if (keycode == XK_Escape)
	{
		win_clean_free(fdf);
		exit(0);
	}
	return (0);
}

int	click_close(t_fdf *fdf)
{
	win_clean_free(fdf);
	exit(0);
	return (0);
}

void	key_close(t_fdf *fdf)
{
	mlx_hook(fdf->win_ptr, 17, 1L<<17, esc_close, &fdf);
	mlx_hook(fdf->win_ptr, 2, 1L<<19, click_close, &fdf);
}



