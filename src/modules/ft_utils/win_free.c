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
		free(fdf->cam_ptr);
		if (fdf->img_ptr)
		{
			if (fdf->img_ptr->line_segment)
				free(fdf->img_ptr->line_segment);
			free(fdf->img_ptr);
		}
		if (fdf->map_data)
		{
			free_points(fdf->map_data->points, fdf->map_data->max_m);
			free(fdf->map_data);
		}
		free(fdf);
	}
}

// Called when window X button is clicked
// Just a notification hook - cleanup happens after mlx_loop returns
void	close_window(void *param)
{
	(void)param;
}
