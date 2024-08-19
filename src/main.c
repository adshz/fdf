/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 10:37:35 by szhong            #+#    #+#             */
/*   Updated: 2024/08/19 17:42:22 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fdf.h"

int	main(int argc, char *argv[])
{
	t_fdf	*fdf;

	if (argc != 2)
		error_handler(1);
	fdf = fdf_init(argv[1]);
	render_data(fdf);
	mlx_key_hook(fdf->win_ptr, &key_handler, fdf);
	mlx_hook(fdf->win_ptr, 17, 0, &close_window, fdf);
	mlx_loop(fdf->mlx_ptr);
	clean_free(fdf);
}
