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
