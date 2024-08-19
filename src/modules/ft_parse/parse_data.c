/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 parse_data.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/07/29 16:14:47 by szhong			   #+#	  #+#			  */
/*	 Updated: 2024/08/01 11:48:26 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "fdf.h"

static t_map	*process_file_content(char *file_content)
{
	t_map	*data;

	data = map_init();
	if (!data)
		return (NULL);
	count_dimensions(file_content, data);
	if (data->max_m == 0 || data->max_n == 0)
	{
		free(data);
		return (NULL);
	}
	data->points = cartesian_init(data->max_n, data->max_m);
	if (!data->points)
	{
		free(data);
		return (NULL);
	}
	parse_content(file_content, data);
	if (data->parse_error)
	{
		free_points(data->points, data->max_m);
		free(data);
		return (NULL);
	}
	return (data);
}

t_map	*parse_data(char *filepath)
{
	char	*file_content;
	t_map	*data;

	data = NULL;
	file_content = get_whole_file(filepath);
	if (!file_content || !*file_content)
	{
		free(data);
		if (file_content)
			free(file_content);
		return (NULL);
	}
	data = process_file_content(file_content);
	free(file_content);
	return (data);
}
