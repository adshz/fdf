/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 15:52:07 by szhong            #+#    #+#             */
/*   Updated: 2024/08/15 16:53:27 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fdf.h"

// Create on-screen menu (called once at startup)
void	print_menu(t_fdf *fdf)
{
	static int	created = 0;
	mlx_t		*mlx;
	int			y;

	if (created)
		return ;
	mlx = (mlx_t *)fdf->mlx;
	y = 20;
	mlx_put_string(mlx, "=== Controls ===", 10, y);
	mlx_put_string(mlx, "ESC: Exit", 10, y += 20);
	mlx_put_string(mlx, "PgUp/Dn: Zoom", 10, y += 20);
	mlx_put_string(mlx, "W/S: Up/Down", 10, y += 20);
	mlx_put_string(mlx, "A/D: Left/Right", 10, y += 20);
	mlx_put_string(mlx, "Arrows: Rotate", 10, y += 20);
	mlx_put_string(mlx, "=/-: Thickness", 10, y += 20);
	mlx_put_string(mlx, "I: Isometric", 10, y += 20);
	mlx_put_string(mlx, "P: Perspective", 10, y += 20);
	mlx_put_string(mlx, "T: Top view", 10, y += 20);
	mlx_put_string(mlx, "Space: Color", 10, y += 20);
	mlx_put_string(mlx, "R: Reset", 10, y += 20);
	created = 1;
}
