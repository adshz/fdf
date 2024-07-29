/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 14:19:33 by szhong            #+#    #+#             */
/*   Updated: 2024/07/29 15:30:46 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fdf.h"
#include "libft.h"

t_map	*map_init(void)
{
	t_map	*result;

	result = (t_map *)malloc(sizeof(t_map) * 1);
	if (!result)
		return (NULL);
	result->max_m = 0;
	result->max_n = 0;
	result->max_z = 0;
	result->min_z = 0;
	result->points = NULL;
	return (result);
}

t_cartesian	**cartesian_init(int max_width, int max_depth)
{
	t_cartesian		**points;
	int				i;

	points = (t_cartesian **)ft_calloc(max_depth + 1, sizeof(t_cartesian *));
	if (points == NULL)
		return (NULL);
	i = 0;
	while (i < max_depth)
	{
		points[i] = ft_calloc(max_width + 1, sizeof(t_cartesian));
		if (!points[i])
		{
			free_points(points, max_depth + 1);
			return (NULL);
		}
		points[i][max_width].x = -1;
		i++;
	}
	points[max_depth] = NULL;
	return (points);
}
//t_cartesian	**cartesian_init(int max_width, int max_depth)
//{
//	t_cartesian		**points;
//	int				i;
//	int				j;
//
//	points = (t_cartesian **)malloc(sizeof(t_cartesian *) * (max_depth + 1));
//	if (points == NULL)
//		return (NULL);
//	i = 0;
//	while (i++ < max_depth)
//	{
//		points[i] = malloc(sizeof(t_cartesian) * (max_width + 1));
//		if (!points[i])
//		{
//			free_points(points, max_depth + 1);
//			return (NULL);
//		}
//		j = 0;
//		while (j++ < max_width)
//		{
//			points[i][j].x = 0.0f;
//			points[i][j].y = 0.0f;
//			points[i][j].z = 0.0f;
//			points[i][j].colour = 0;
//		}
//		points[i][max_width].x = -1;
//	}
//	points[max_depth] = NULL;
//	return (points);
//}
//
