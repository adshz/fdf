/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 error_and_free.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/07/29 13:18:08 by szhong			   #+#	  #+#			  */
/*	 Updated: 2024/07/29 13:19:10 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "fdf.h"
#include "libft.h"

void	clean_up(t_fdf **fdf)
{
	if ((*fdf)->map_data == NULL || (*fdf)->img_ptr == NULL || \
			(*fdf)->cam_ptr == NULL || (*fdf)->mlx_ptr == NULL \
			|| (*fdf)->win_ptr == NULL)
	{
		clean_free((*fdf));
		error_handler(3);
	}
}

void	clean_free(t_fdf *fdf)
{
	if (fdf)
	{
		if (fdf->img_ptr->line_segment)
			free(fdf->img_ptr->line_segment);
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
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	return ;
}

void	free_points(t_cartesian **points, int max_depth)
{
	int	i;

	if (points == NULL)
		return ;
	i = 0;
	while (i < max_depth + 1)
	{
		free(points[i]);
		i++;
	}
	free(points);
	return ;
}

void	error_handler(int code)
{
	if (code == 0)
		ft_putendl_fd("The End of Program. See you Next time!", 1);
	else if (code == 1)
	{
		ft_putendl_fd("ERROR: Invalid Number of Argument", 2);
		ft_putendl_fd("Usage: ./fdf <Path to the filename>", 1);
		exit(0);
	}
	else if (code == 2)
		ft_putendl_fd("ERROR: Invalid Input File", 2);
	else if (code == 3)
		ft_putendl_fd("ERROR: Initialisation", 2);
	else if (code == 4)
		ft_putendl_fd("ERROR: Data Parsing Failure", 2);
	else if (code == 5)
		ft_putendl_fd("ERROR: Data Rendering Failure", 2);
	else if (code == 6)
		ft_putendl_fd("ERROR", 2);
	exit(code);
}
