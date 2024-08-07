/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 utils.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/08/01 15:15:49 by szhong			   #+#	  #+#			  */
/*	 Updated: 2024/08/01 16:27:53 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "fdf.h"

void	clean_up(t_fdf	**fdf)
{
	if ((*fdf)->map_data == NULL || (*fdf)->img_ptr == NULL || \
			(*fdf)->cam_ptr == NULL || (*fdf)->mlx_ptr == NULL\
			|| (*fdf)->win_ptr == NULL)
	{
		clean_free((*fdf));
		exit(1);
	}
}

void	clean_free(t_fdf *fdf)
{
	if (fdf)
	{
		if (fdf->mlx_ptr && fdf->win_ptr)
			mlx_destroy_window(fdf->mlx_ptr, fdf->win_ptr);
		if (fdf->mlx_ptr && fdf->img_ptr && fdf->img_ptr->img_buff)
			mlx_destroy_image(fdf->mlx_ptr, fdf->img_ptr->img_buff);
		if (fdf->mlx_ptr)
		{
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
