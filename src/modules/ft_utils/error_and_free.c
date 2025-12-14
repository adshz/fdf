/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_and_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 13:18:08 by szhong            #+#    #+#             */
/*   Updated: 2024/07/29 13:19:10 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fdf.h"
#include "libft.h"

void	clean_up(t_fdf **fdf)
{
	if ((*fdf)->map_data == NULL || (*fdf)->img_ptr == NULL || \
			(*fdf)->cam_ptr == NULL || (*fdf)->mlx == NULL)
	{
		clean_free((*fdf));
		error_handler(3);
	}
}

void	clean_free(t_fdf *fdf)
{
	if (fdf)
	{
		if (fdf->img_ptr && fdf->img_ptr->line_segment)
			free(fdf->img_ptr->line_segment);
		if (fdf->mlx)
			mlx_terminate(fdf->mlx);
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

static void	print_usage(void)
{
	ft_putendl_fd("\033[1;36m", 1);
	ft_putendl_fd("  ███████╗██████╗ ███████╗", 1);
	ft_putendl_fd("  ██╔════╝██╔══██╗██╔════╝", 1);
	ft_putendl_fd("  █████╗  ██║  ██║█████╗  ", 1);
	ft_putendl_fd("  ██╔══╝  ██║  ██║██╔══╝  ", 1);
	ft_putendl_fd("  ██║     ██████╔╝██║     ", 1);
	ft_putendl_fd("  ╚═╝     ╚═════╝ ╚═╝     \033[0m", 1);
	ft_putendl_fd("", 1);
	ft_putendl_fd("\033[1;33mUsage:\033[0m ./fdf <map_file>", 1);
	ft_putendl_fd("", 1);
	ft_putendl_fd("\033[1;33mExamples:\033[0m", 1);
	ft_putendl_fd("  ./fdf maps/42.fdf", 1);
	ft_putendl_fd("  ./fdf maps/pyramide.fdf", 1);
	ft_putendl_fd("  ./fdf maps/elem-col.fdf", 1);
	ft_putendl_fd("", 1);
	ft_putendl_fd("\033[1;33mAvailable maps:\033[0m maps/", 1);
}

void	error_handler(int code)
{
	if (code == 0)
		ft_putendl_fd("\033[1;32mGoodbye!\033[0m", 1);
	else if (code == 1)
	{
		ft_putendl_fd("\033[1;31mError: Invalid number of arguments\033[0m", 2);
		print_usage();
		exit(0);
	}
	else if (code == 2)
		ft_putendl_fd("\033[1;31mError: Cannot open file\033[0m", 2);
	else if (code == 3)
		ft_putendl_fd("\033[1;31mError: Initialization failed\033[0m", 2);
	else if (code == 4)
		ft_putendl_fd("\033[1;31mError: Invalid map format\033[0m", 2);
	else if (code == 5)
		ft_putendl_fd("\033[1;31mError: Rendering failed\033[0m", 2);
	else if (code == 6)
		ft_putendl_fd("\033[1;31mError\033[0m", 2);
	exit(code);
}
