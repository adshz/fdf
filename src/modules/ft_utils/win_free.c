/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   win_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 15:59:37 by szhong            #+#    #+#             */
/*   Updated: 2024/08/16 16:41:20 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fdf.h"

void	win_clean_free(t_fdf *fdf)
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
