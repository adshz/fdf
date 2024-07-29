/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 14:49:04 by szhong            #+#    #+#             */
/*   Updated: 2024/07/29 15:13:17 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fdf.h"
#include <stdio.h>

void	print_map(t_cartesian **points, int max_width, int max_depth)
{
	int	i;
	int	j;

	i = 0;
	while (i < max_depth)
	{
		j = 0;
		while (j < max_width)
		{
			printf("x is %f\ty is %f\tz is %f\tcolour is %d\n", \
					points[i][j].x, points[i][j].y, points[i][j].z, points[i][j].colour);
			j++;
		}
		i++;
	}
	return ;
}


