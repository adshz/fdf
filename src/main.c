/* ************************************************************************** */
/*									      */
/*							  :::	   ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*						      +:+ +:+	      +:+     */
/*   By: szhong <marvin@42.fr>			    +#+  +:+	   +#+	      */
/*						  +#+#+#+#+#+	+#+	      */
/*   Created: 2024/07/11 13:59:19 by szhong	       #+#    #+#	      */
/*   Updated: 2024/08/14 13:34:21 by szhong           ###   ########.fr       */
/*									      */ 
/* ************************************************************************** */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <link.h>
#include <math.h>
#include <fcntl.h>
#include "libft.h"
#include "fdf.h"
#include <assert.h> 
#include <stdio.h>

#include "../minilibx-linux/mlx.h"

int	main(int argc, char *argv[])
{
	t_fdf	*fdf;

	if (argc != 2)
		ft_putendl_fd("ERROR", 2);
	fdf = fdf_init(argv[1]);
	render_data(fdf);
	mlx_key_hook(fdf->win_ptr, &key_handler, fdf);
//	mlx_mouse_hook(fdf->win_ptr, &mouse_handler, fdf);
	mlx_hook(fdf->win_ptr, 17, 0, &close_window, fdf);
	mlx_loop(fdf->mlx_ptr);
	clean_free(fdf);
}
